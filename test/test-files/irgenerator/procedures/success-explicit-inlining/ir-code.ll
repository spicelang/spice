; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [21 x i8] c"This is a value: %d\0A\00", align 1
@1 = private unnamed_addr constant [14 x i8] c"Before value\0A\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"After value\0A\00", align 1

; Function Attrs: alwaysinline
define internal void @_p__void__void__printAValue() #0 {
entry.l1:
  %0 = call i32 (ptr, ...) @printf(ptr @0, i32 5)
  ret void
}

declare i32 @printf(ptr, ...)

define i32 @main() {
entry.l5:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @1)
  call void @_p__void__void__printAValue()
  store i1 true, ptr %0, align 1
  %2 = call i32 (ptr, ...) @printf(ptr @2)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

attributes #0 = { alwaysinline }
