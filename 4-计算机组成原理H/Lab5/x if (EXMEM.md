```rust
if (EX/MEM.RegWrite 
    and (EX/MEM.RegisterRd ≠ 0) 
    and (EX/MEM.RegisterRd = ID/EX.RegisterRs1)) ForwardA = 10
if (EX/MEM.RegWrite 
    and (EX/MEM.RegisterRd ≠ 0) 
    and (EX/MEM.RegisterRd = ID/EX.RegisterRs2)) ForwardB = 10
```

```rust
if (MEM/WB.RegWrite 
    and (MEM/WB.RegisterRd ≠ 0) 
    and not (EX/MEM.RegWrite and (EX/MEM.RegisterRd ≠ 0)
    	and (EX/MEM.RegisterRd ≠ ID/EX.RegisterRs1))
    and (MEM/WB.RegisterRd = ID/EX.RegisterRs1)) ForwardA = 01
if (MEM/WB.RegWrite 
    and (MEM/WB.RegisterRd ≠ 0) 
    and not (EX/MEM.RegWrite and (EX/MEM.RegisterRd ≠ 0)
    	and (EX/MEM.RegisterRd ≠ ID/EX.RegisterRs2))
    and (MEM/WB.RegisterRd = ID/EX.RegisterRs2)) ForwardB = 01
```

```python
if (ID/EX.MemRead
    and ((ID/EX.RegisterRd = IF/ID.RegisterRs1)
        or (ID/EX.RegisterRd = IF/ID.RegisterRs2))) Hazard = 1
```

