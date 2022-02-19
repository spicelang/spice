; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [19 x i8] c"Inlined value: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: alwaysinline nounwind
define internal i64 @"getInlinedValue()"() #0 {
entry:
  %result = alloca i64, align 8
  %0 = alloca i64, align 8
  store i64 12, i64* %0, align 8
  %1 = load i64, i64* %0, align 8
  ret i64 %1
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i64, align 8
  store i32 0, i32* %result, align 4
  %1 = call i64 @"getInlinedValue()"()
  store i64 %1, i64* %0, align 8
  %2 = load i64, i64* %0, align 8
  %3 = trunc i64 %2 to i32
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @0, i32 0, i32 0), i32 %3)
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}

attributes #0 = { alwaysinline nounwind }
