; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [16 x i8] c"Global var: %s\0A\00", align 1
@GLOBAL = external global ptr

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = load ptr, ptr @GLOBAL, align 8
  %1 = alloca ptr, align 8
  store ptr %0, ptr %1, align 8
  %2 = load ptr, ptr %1, align 8
  %3 = call i32 (ptr, ...) @printf(ptr @0, ptr %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare i32 @printf(ptr, ...)
