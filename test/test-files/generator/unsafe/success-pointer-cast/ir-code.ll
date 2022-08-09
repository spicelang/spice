; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %l = alloca i64, align 8
  %0 = alloca ptr, align 8
  %iPtr = alloca ptr, align 8
  %1 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i64 1234, ptr %l, align 8
  store ptr %l, ptr %0, align 8
  store ptr null, ptr %iPtr, align 8
  %2 = load ptr, ptr %0, align 8
  store ptr %2, ptr %1, align 8
  %3 = load ptr, ptr %1, align 8
  store ptr %3, ptr %iPtr, align 8
  %4 = load ptr, ptr %iPtr, align 8
  %5 = load i32, ptr %4, align 4
  store i32 %5, ptr %4, align 4
  %6 = load i32, ptr %4, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @0, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare i32 @printf(ptr, ...)
