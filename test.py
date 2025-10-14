import subprocess

MINISHELL_PATH = "./minishell"
COMMANDS = [
    "echo hello",
    "ls -l",
    "pwd",
    "cat /etc/passwd | head -2",
    "cat envp_demo | (head -1 && cat) | wc -c",
    "echo hello",
    "ls -l",
    "pwd",
    "cat /etc/passwd | head -2",
    "export FOO=bar && echo $FOO",
    "unset FOO && echo $FOO",
    "false",
    "true",
    "exit 42",
    "echo \"quoted string\"",
    "echo hello world > /tmp/testfile",
    "cat < /tmp/testfile",
    "ls | grep minishell",
    "echo $HOME",
    "cd /tmp && pwd",
    "echo foo && echo bar",
    "echo foo || echo bar",
    "(echo subshell)",
    "echo foo > /tmp/a && cat /tmp/a",
    "rm -f /tmp/testfile /tmp/a",
    "echo \$((2 + 2))",
    "touch file1 file2 && ls file*",
    "echo \"multi\nline\"",
    "echo -n \"no newline\"",
    "ls /doesnotexist",
    "echo $PATH",
    "export VAR1=abc VAR2=def && echo $VAR1 $VAR2",
    "unset VAR1 VAR2 && echo $VAR1 $VAR2",
    "echo 'single quoted'",
    "echo \"double quoted\"",
    "echo \\$SHELL",
    "echo \`whoami\`",
    "history",
    "echo foo | tee /tmp/foo",
    "cat /tmp/foo",
    "rm -f /tmp/foo",
    "echo \"Mix 'quotes' and \$USER: $USER\"",
    "echo \"Escaped \\\"double quotes\\\" and \$PATH\"",
]

def run_shell(shell_cmd, command, use_argv=False):
    try:
        if use_argv:
            proc = subprocess.run(
                [MINISHELL_PATH, command],
                text=True,
                capture_output=True,
                timeout=5,
                shell=False
            )
        else:
            proc = subprocess.run(
                shell_cmd,
                input=command,
                text=True,
                capture_output=True,
                timeout=5,
                shell=False
            )
        return proc.stdout, proc.stderr, proc.returncode
    except Exception as e:
        return "", str(e), -1

def main():
    for cmd in COMMANDS:
        print(f"Testing: {cmd}")
        bash_out, bash_err, bash_code = run_shell(["bash", "-c", cmd], "", use_argv=False)
        mini_out, mini_err, mini_code = run_shell([], cmd, use_argv=True)

        print(f"    Bash:      [{bash_out.strip()}] [{bash_err.strip()}] (code {bash_code})")
        print(f"    Minishell: [{mini_out.strip()}] [{mini_err.strip()}] (code {mini_code})")

        if (bash_out, bash_err, bash_code) == (mini_out, mini_err, mini_code):
            print("  ✅ Output and exit code match")
        else:
            print("  ❌ Difference found:")
        print()

if __name__ == "__main__":
    main()