; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry.l9:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca i32, align 4
  store i32 12, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = call i32 (ptr, ...) @printf(ptr @0, i32 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

declare i32 @printf(ptr, ...)
