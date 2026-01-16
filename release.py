#!/usr/bin/env python3
import subprocess
import sys
import os
import platform
import shutil
from pathlib import Path

# ==========================================
# CONFIGURATION
# ==========================================
PROJECT_NAME = "MyProject"
BUILD_DIR = "build"
BUILD_TYPE = "Release"

# GitHub CLI settings
GITHUB_REPO_OWNER = ""  # Leave empty if 'gh' can infer from git remote
GITHUB_REPO_NAME = ""  # Leave empty if 'gh' can infer from git remote

# ==========================================
# HELPER FUNCTIONS
# ==========================================

def run_command(cmd, check=True):
    """Runs a shell command and prints output."""
    print(f">> Running: {' '.join(cmd)}")
    try:
        subprocess.run(cmd, check=check)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command '{e.cmd}' returned non-zero exit status {e.returncode}.")
        sys.exit(1)

def confirm(prompt):
    """Asks user for Y/N confirmation."""
    try:
        ans = input(prompt + " (y/n): ").lower()
        return ans in ['y', 'yes']
    except KeyboardInterrupt:
        print("\nAborted.")
        sys.exit(1)

def get_os_info():
    """
    Detects the Operating System to configure CMake generators and extensions.
    Returns: dict with 'name', 'generator', 'extension'
    """
    system = platform.system()
    distro = "Unknown"

    # Check Linux distro specifically
    if system == "Linux":
        try:
            with open("/etc/os-release", "r") as f:
                content = f.read().lower()
                if "ubuntu" in content:
                    distro = "Ubuntu"
                elif "void" in content:
                    distro = "Void Linux"
                elif "arch" in content:
                    distro = "Arch"
                elif "debian" in content:
                    distro = "Debian"
                else:
                    distro = "Generic Linux"
        except FileNotFoundError:
            distro = "Generic Linux"

    if system == "Windows":
        return {
            "name": "Windows",
            "generator": "MinGW Makefiles", # Or "Ninja" if you prefer
            "extension": ".exe"
        }
    elif system == "Darwin":
        return {
            "name": "macOS",
            "generator": "Unix Makefiles", # Or "Xcode"
            "extension": ""
        }
    elif system == "Linux":
        # For Ubuntu and Void, Unix Makefiles is the standard CMake default
        return {
            "name": distro,
            "generator": "Unix Makefiles",
            "extension": ""
        }
    else:
        print(f"Error: Unsupported OS detected: {system}")
        sys.exit(1)

# ==========================================
# 1. INPUT VALIDATION
# ==========================================

if len(sys.argv) < 2:
    print("Error: No version tag provided.")
    print("Usage: python release.py <version> (e.g., 1.0.0)")
    sys.exit(1)

VERSION_TAG = f"v{sys.argv[1]}"
ARTIFACT_NAME = f"{PROJECT_NAME}-{VERSION_TAG}{get_os_info()['extension']}"

print("-" * 40)
print(f"Starting Release Process: {VERSION_TAG}")
print("-" * 40)

# ==========================================
# 2. GIT TAGGING
# ==========================================

def manage_git_tag(tag):
    # Check if tag exists locally
    result = subprocess.run(["git", "tag", "-l", tag], capture_output=True, text=True)
    if tag in result.stdout:
        print(f"Warning: Tag {tag} already exists locally.")
        if confirm(f"Do you want to delete the old local tag '{tag}' and continue?"):
            run_command(["git", "tag", "-d", tag])
        else:
            print("Aborting.")
            sys.exit(1)

    print(f"Creating git tag: {tag}")
    run_command(["git", "tag", "-a", tag, "-m", f"Release {tag}"])

    print("Pushing tag to origin")
    run_command(["git", "push", "origin", tag])

manage_git_tag(VERSION_TAG)

# ==========================================
# 3. COMPILATION
# ==========================================

def build_project(os_info):
    print(f"Compiling project for {os_info['name']}...")

    build_path = Path(BUILD_DIR)

    # Create build directory
    build_path.mkdir(exist_ok=True)

    # Change directory to build
    # Note: Python's subprocess.run allows us to specify cwd without changing the global script dir
    cwd = os.getcwd()
    os.chdir(build_path)

    try:
        # 1. Configure
        # Note: We use -G to specify the generator.
        # If you prefer to let CMake decide, remove the `-G` argument.
        cmake_cmd = [
            "cmake",
            "..",
            "-G", os_info['generator'],
            f"-DCMAKE_BUILD_TYPE={BUILD_TYPE}"
        ]
        run_command(cmake_cmd)

        # 2. Build
        # 'cmake --build' is the modern cross-platform way to build
        build_cmd = ["cmake", "--build", ".", "--config", BUILD_TYPE]
        run_command(build_cmd)

        print("Build successful!")

    finally:
        # Return to original directory
        os.chdir(cwd)

build_project(get_os_info())

# ==========================================
# 4. PREPARE ARTIFACT
# ==========================================

def find_artifact(os_info):
    """
    Locates the compiled executable.
    Checks standard locations: root of build dir or 'Release' subfolder.
    """
    exe_name = f"{PROJECT_NAME}{os_info['extension']}"

    # Possible locations where CMake puts the binary
    possible_paths = [
        Path(BUILD_DIR) / BUILD_TYPE / exe_name,  # Windows/VS Code default multi-config
        Path(BUILD_DIR) / exe_name                # Linux/MinGW single-config
    ]

    exec_path = None
    for p in possible_paths:
        if p.exists():
            exec_path = p
            break

    if not exec_path:
        print(f"Error: Could not find {exe_name} in {BUILD_DIR}/")
        print("Files in build directory:")
        # Run ls recursively to debug
        if platform.system() == "Windows":
            run_command(["dir", "/s", "/b", BUILD_DIR], check=False)
        else:
            run_command(["find", BUILD_DIR, "-type", "f"], check=False)
        sys.exit(1)

    print(f"Found artifact: {exec_path}")
    return exec_path

EXECUTABLE_PATH = find_artifact(get_os_info())

# ==========================================
# 5. CREATE GITHUB RELEASE
# ==========================================

def create_github_release(tag, artifact_path, artifact_name):
    print("Creating GitHub Release...")

    # Check if 'gh' is installed
    if not shutil.which("gh"):
        print("Error: GitHub CLI (gh) is not installed or not in PATH.")
        print("Please install it: https://cli.github.com/")
        sys.exit(1)

    # Create the release and upload
    # Syntax: gh release create <tag> <file>#<new_name> --title ...
    upload_spec = f"{artifact_path}#{artifact_name}"

    cmd = [
        "gh", "release", "create", tag,
        upload_spec,
        "--title", f"Release {tag}",
        "--notes", f"Automated release for version {tag} on {platform.system()}"
    ]

    run_command(cmd)

create_github_release(VERSION_TAG, EXECUTABLE_PATH, ARTIFACT_NAME)

print("-" * 40)
print(f"Release {VERSION_TAG} created successfully!")
print("-" * 40)