import subprocess
import os
import sys

def find_executable(executable, path=None):
    """Find if an executable exists in the PATH."""
    if path is None:
        path = os.environ["PATH"]
    paths = path.split(os.pathsep)
    base, ext = os.path.splitext(executable)

    if sys.platform == "win32" and not ext:
        executable = executable + ".exe"

    for p in paths:
        full_path = os.path.join(p, executable)
        if os.path.exists(full_path) and os.access(full_path, os.X_OK):
            return full_path

    return None

# Use it like this:
plugin_path = find_executable('protoc-gen-nanopb')
if plugin_path is None:
    raise EnvironmentError("protoc-gen-nanopb not found in PATH.")

# Now you can use plugin_path in your command

proto_files_dir = os.path.join('..', '..', 'intelligence', 'messages')# Directory containing your .proto file
output_dir = os.path.join('src', 'messages')  # Output directory for the generated files

# Ensure the output directory exists
os.makedirs(output_dir, exist_ok=True)

# Full path to your .proto file
proto_files = [f for f in os.listdir(proto_files_dir) if f.endswith('.proto')]

def compile_proto(proto_file: str, out_path: str):
	command = [
		'protoc',
		f'--plugin=protoc-gen-nanopb={plugin_path}',
		f'--nanopb_out={out_path}',
		f'--proto_path={proto_files_dir}',
		# '--nanopb_opt=-v',
		proto_file
	]
	subprocess.run(command, check=True)

for file in proto_files:
	print(f'Compiling {file}')
	compile_proto(file, output_dir)

# Custom output messages
compile_proto('remote.proto', os.path.join('lib', 'krakenbridge', 'src'))

# Run the command
