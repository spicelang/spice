; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Param: %d\0A\00", align 1

define private void @_p__void__void__test__int(i32 %0) {
  %param = alloca i32, align 4
  store i32 %0, ptr %param, align 4
  %2 = load i32, ptr %param, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  ret void
}

declare i32 @printf(ptr noundef, ...)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_p__void__void__test__int(i32 12)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { noinline nounwind optnone uwtable }