#!/usr/bin/env python3
"""Download third-party dependencies (git submodules + ANTLR jar)."""
import subprocess
import urllib.request
from pathlib import Path

ANTLR_VERSION = "4.13.2"

subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)

jar_path = Path("src/thirdparty") / f"antlr-{ANTLR_VERSION}-complete.jar"
if not jar_path.exists():
    url = f"https://www.antlr.org/download/antlr-{ANTLR_VERSION}-complete.jar"
    print(f"Downloading {url} ...")
    urllib.request.urlretrieve(url, jar_path)
