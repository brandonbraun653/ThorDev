from argparse import ArgumentParser
from pathlib import Path
from loguru import logger
import subprocess


def invoke_mock_generator(config_file: Path, input_file: Path, mock_output: Path, expect_output: Path) -> None:
    assert config_file.exists()
    assert input_file.exists()

    cmd = ["CppUMockGen",
           "--config-file", config_file.as_posix(),
           "--input", input_file.as_posix(),
           "--mock-output", mock_output.as_posix(),
           "--expect-output", expect_output.as_posix()]
    logger.info(f"Running command: \r\n{' '.join(cmd)}")
    result = subprocess.run(cmd, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)

    if result.stdout:
        print(result.stdout.decode("utf-8"))
    if result.stderr:
        print(result.stderr.decode("utf-8"))


def generate_mock_from_header_file(input_file: Path) -> None:
    if input_file.suffix != ".h" and input_file.suffix != ".hpp":
        logger.error(f"Input file {input_file} is not a header file")
        return

    relative_path = input_file.relative_to(Path.cwd())
    config_file = Path.cwd() / "mock" / "config" / "root.cfg"
    out_location = Path.cwd() / "mock" / relative_path.parent
    out_location.mkdir(parents=True, exist_ok=True)

    invoke_mock_generator(config_file, input_file, out_location, out_location)


def main() -> None:
    parser = ArgumentParser()
    parser.add_argument("--all", action="store_true", required=False)
    parser.add_argument("--file", type=str, required=False)

    args = parser.parse_args()
    if args.all:
        logger.error("Not implemented yet")
    elif args.file:
        generate_mock_from_header_file(Path(args.file))


if __name__ == "__main__":
    main()
