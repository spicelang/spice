; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__double__Vector__doubleptr_int = type { ptr, i32 }

@printf.str.0 = private unnamed_addr constant [24 x i8] c"Capacity of vector: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %dbl = alloca double, align 8
  %doubleVec = alloca %__double__Vector__doubleptr_int, align 8
  store i32 0, ptr %result, align 4
  store double 3.467000e+00, ptr %dbl, align 8
  %1 = getelementptr inbounds %__double__Vector__doubleptr_int, ptr %doubleVec, i32 0, i32 0
  store ptr %dbl, ptr %1, align 8
  %2 = getelementptr inbounds %__double__Vector__doubleptr_int, ptr %doubleVec, i32 0, i32 1
  store i32 1, ptr %2, align 4
  %cap = getelementptr inbounds %__double__Vector__doubleptr_int, ptr %doubleVec, i32 0, i32 1
  %3 = load i32, ptr %cap, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
