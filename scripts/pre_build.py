import subprocess

Import("env")

#env.AddPostAction("$PROGPATH", post_program_action)

def get_git_revision_hash() -> str:
    return subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('ascii').strip()

def get_git_revision_short_hash() -> str:
    return subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode('ascii').strip()

def ensureGitHashValid():
    with open("scripts/fw_version_git_hash.txt", 'w+') as gitHashFile:
        previousGitHash = gitHashFile.readline()

        if(previousGitHash == ""):
            gitHashFile.write(get_git_revision_hash())
            
        print(previousGitHash)
    
    return


def checkAndUpdateGitHash():

    return


def main():

    ensureGitHashValid()

    checkAndUpdateGitHash()


main()