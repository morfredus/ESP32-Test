import json
import os
import re
import subprocess
import sys
from pathlib import Path

# Map detection to PlatformIO env names
ENV_MAP = {
    ("ESP32-S3", 16, True): "esp32s3_n16r8",
    ("ESP32-S3", 8, True): "esp32s3_n8r8",
    ("ESP32", 4, False): "esp32devkitc",
}

PORT_HINTS = ["COM", "/dev/ttyUSB", "/dev/ttyACM"]

ROOT = Path(__file__).resolve().parents[1]


def run_cmd(args):
    return subprocess.run(args, capture_output=True, text=True)


def list_ports():
    """Use `platformio device list --json-output` to enumerate ports."""
    r = run_cmd(["platformio", "device", "list", "--json-output"]) 
    if r.returncode != 0:
        return []
    try:
        return json.loads(r.stdout)
    except Exception:
        return []


def detect_chip_and_flash(port: str):
    """Use esptool to read chip and flash size.
    Returns (chip, flash_mb, psram_available) or None.
    """
    # Try using esptool.py from PlatformIO penv
    esptool_cmd = ["esptool.py", "--port", port, "chip_id"]
    r = run_cmd(esptool_cmd)
    chip = None
    if r.returncode == 0:
        if "ESP32-S3" in r.stdout:
            chip = "ESP32-S3"
        elif "ESP32" in r.stdout:
            chip = "ESP32"
    # read_flash_id to infer size (heuristic)
    r2 = run_cmd(["esptool.py", "--port", port, "flash_id"])
    flash_mb = None
    if r2.returncode == 0:
        m = re.search(r"Detected flash size:\s*(\d+)MB", r2.stdout)
        if m:
            flash_mb = int(m.group(1))
    # PSRAM check via esptool not straightforward; try idf.py not available.
    # Heuristic: S3 boards often have PSRAM; if flash >=8MB and chip is ESP32-S3, assume PSRAM.
    psram = bool(chip == "ESP32-S3" and (flash_mb or 0) >= 8)
    if chip and flash_mb:
        return chip, flash_mb, psram
    return None


def choose_env():
    ports = list_ports()
    # Prefer USB CDC / active ports
    for p in ports:
        port = p.get("port") or p.get("address")
        if not port:
            continue
        if not any(port.startswith(h) for h in PORT_HINTS):
            continue
        info = detect_chip_and_flash(port)
        if info:
            chip, flash_mb, psram = info
            key = (chip, 16 if flash_mb >= 16 else 8 if flash_mb >= 8 else 4, psram)
            env = ENV_MAP.get(key)
            if env:
                return env, port
            # Fallbacks
            if chip == "ESP32-S3":
                env = "esp32s3_n16r8" if flash_mb and flash_mb >= 16 else "esp32s3_n8r8"
                return env, port
            else:
                return "esp32devkitc", port
    return None, None


def main():
    if len(sys.argv) < 2:
        print("Usage: python scripts/auto_env.py [upload|monitor|build]")
        sys.exit(1)
    action = sys.argv[1]
    env, port = choose_env()
    if not env:
        print("Could not auto-detect environment. Please set default_envs in platformio.ini.")
        sys.exit(2)
    print(f"Detected env: {env} on port {port}")
    # Run PlatformIO with selected env
    if action == "upload":
        cmd = ["platformio", "run", "-t", "upload", "-e", env]
    elif action == "monitor":
        cmd = ["platformio", "device", "monitor", "-b", "115200", "-p", port]
    elif action == "build":
        cmd = ["platformio", "run", "-e", env]
    else:
        print("Unknown action. Use upload|monitor|build")
        sys.exit(3)
    print("Running:", " ".join(cmd))
    r = subprocess.run(cmd)
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
