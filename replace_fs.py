import os
from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

# Get the project directory
project_dir = env.subst("$PROJECT_DIR")

# Path to the LittleFS image
fs_image_path = os.path.join(project_dir, ".pio", "build", "esp32doit-devkit-v1", "littlefs.bin")

# Function to remove the LittleFS image before build
def before_buildfs(source, target, env):
    if os.path.exists(fs_image_path):
        print("Removing old LittleFS image...")
        os.remove(fs_image_path)

# Add the pre-action to the buildfs target
env.AddPreAction("$BUILD_DIR/littlefs.bin", before_buildfs)
