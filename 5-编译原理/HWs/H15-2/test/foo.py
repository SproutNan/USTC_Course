def trampoline(args, flag=1):
    assert flag <= 1, "illegal flag"
    if flag == 1:
        assert len(args) >= 2, "args length unmatched"
        if args[0] == 1:
            return args[1]
        _args = [args[0] - 1, args[1] * (args[0] % 3 + 1)]
        return trampoline(_args, flag)

if __name__ == "__main__":
    print(trampoline([10, 1], 1))