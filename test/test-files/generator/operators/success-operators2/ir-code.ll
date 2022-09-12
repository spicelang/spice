; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [14 x i8] c"Result 1: %d\0A\00", align 1
@1 = private unnamed_addr constant [14 x i8] c"Result 2: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 1, ptr %i, align 4
  %0 = load i32, ptr %i, align 4
  %1 = add i32 %0, 2
  store i32 %1, ptr %i, align 4
  %2 = load i32, ptr %i, align 4
  %3 = mul i32 %2, 2
  store i32 %3, ptr %i, align 4
  %4 = load i32, ptr %i, align 4
  %5 = sdiv i32 %4, 2
  store i32 %5, ptr %i, align 4
  %6 = load i32, ptr %i, align 4
  %7 = sub i32 %6, 2
  store i32 %7, ptr %i, align 4
  %8 = load i32, ptr %i, align 4
  %9 = call i32 (ptr, ...) @printf(ptr @0, i32 %8)
  %10 = load i32, ptr %i, align 4
  %11 = add i32 %10, 494665727
  store i32 %11, ptr %i, align 4
  %12 = load i32, ptr %i, align 4
  %13 = sdiv i32 %12, 2
  store i32 %13, ptr %i, align 4
  %14 = load i32, ptr %i, align 4
  %15 = mul i32 %14, 2
  store i32 %15, ptr %i, align 4
  %16 = load i32, ptr %i, align 4
  %17 = sub i32 %16, 494665727
  store i32 %17, ptr %i, align 4
  %18 = load i32, ptr %i, align 4
  %19 = call i32 (ptr, ...) @printf(ptr @1, i32 %18)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
}

declare i32 @printf(ptr, ...)
