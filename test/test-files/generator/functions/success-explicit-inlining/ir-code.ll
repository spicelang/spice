; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [19 x i8] c"Inlined value: %d\0A\00", align 1

; Function Attrs: alwaysinline
define internal i64 @_f__void__getInlinedValue() #0 {
entry.l1:
  %result = alloca i64, align 8
  %0 = alloca i64, align 8
  store i64 12, ptr %0, align 8
  %1 = load i64, ptr %0, align 8
  ret i64 %1
}

define i32 @main() {
entry.l5:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = call i64 @_f__void__getInlinedValue()
  %1 = alloca i64, align 8
  store i64 %0, ptr %1, align 8
  %2 = load i64, ptr %1, align 8
  %3 = call i32 (ptr, ...) @printf(ptr @0, i64 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare i32 @printf(ptr, ...)

attributes #0 = { alwaysinline }
