; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [19 x i8] c"Inlined value: %d\0A\00", align 1

; Function Attrs: alwaysinline
define private i64 @_f__void__long__getInlinedValue() #0 {
  %result = alloca i64, align 8
  ret i64 12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i64 @_f__void__long__getInlinedValue()
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { alwaysinline }
attributes #1 = { noinline nounwind optnone uwtable }
