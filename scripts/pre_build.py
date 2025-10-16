import subprocess
import os

Import("env")

PREFIX = "PREBUILD: "

def get_git_revision_hash() -> str:
    return subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('ascii').strip()

def get_git_revision_short_hash() -> str:
    return subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode('ascii').strip()


# Checks if a hash exists, if not (i.e. first run) writes git hash to file
def ensureGitHashValid():
    hash_file_path = "scripts/fw_version_git_hash.txt"
    
    # Create directory if it doesn't exist
    os.makedirs(os.path.dirname(hash_file_path), exist_ok=True)
    
    # Create file if it doesn't exist
    if not os.path.exists(hash_file_path):
        with open(hash_file_path, 'w') as gitHashFile:
            gitHashFile.write(get_git_revision_hash())
        return
    
    with open(hash_file_path, 'r+') as gitHashFile:
        previousGitHash = gitHashFile.readline().strip()

        if previousGitHash == "":
            gitHashFile.write(get_git_revision_hash())
    
    return


def checkGitHashChanged() -> bool:
    hash_file_path = "scripts/fw_version_git_hash.txt"
    
    # Check if hash is the same
    with open(hash_file_path, 'r') as gitHashFile:
        previousGitHash = gitHashFile.readline().strip()
        currentHash = get_git_revision_hash()

        return previousGitHash != currentHash


def updateFirmwareVersion(hashChanged: bool):

    if not hashChanged:
        print(f"{PREFIX}No firmware version change necessary.")
        return

    # Otherwise, increment firmware version
    device_file_path = "include/device.hpp"
    
    with open(device_file_path, 'r') as deviceFirmwareFile:
        # Get file contents in one read 
        lines = deviceFirmwareFile.readlines()
    
    # Loop through each line looking for our line to update
    updated = False
    for i in range(len(lines)):
        line = lines[i]
        
        # Look for our line
        if "#define FIRMWARE_VERSION_MICRO" in line:
            # Extract the number (handle multi-digit numbers)
            parts = line.strip().split()
            if len(parts) >= 3:
                try:
                    firmwareVersionMicro = int(parts[-1])
                    
                    # Replace the line with updated version
                    lines[i] = ' '.join(parts[:-1]) + ' ' + str(firmwareVersionMicro + 1) + '\n'
                    
                    print(f"{PREFIX}Successfully incremented firmware version micro from {firmwareVersionMicro} to {firmwareVersionMicro+1}")
                    updated = True
                    break
                except ValueError:
                    print(f"{PREFIX}Error: Could not parse firmware version number")
                    return

    if not updated:
        print(f"{PREFIX}Warning: FIRMWARE_VERSION_MICRO not found in {device_file_path}")
        return

    # Write it back
    with open(device_file_path, 'w') as deviceFirmwareFile:
        deviceFirmwareFile.writelines(lines)
    
    # Update the git hash file
    with open("scripts/fw_version_git_hash.txt", 'w') as gitHashFile:
        gitHashFile.write(get_git_revision_hash())
    
    return


def main():
    ensureGitHashValid()

    hashChanged = checkGitHashChanged()
    
    updateFirmwareVersion(hashChanged)


main()