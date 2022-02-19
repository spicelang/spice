; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define internal i32 @"faculty(int)"(i32 %0) #0 {
entry:
  %input = alloca i32, align 4
  %result = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, i32* %input, align 4
  store i32 2, i32* %1, align 4
  %8 = load i32, i32* %input, align 4
  %9 = load i32, i32* %1, align 4
  %10 = icmp slt i32 %8, %9
  store i1 %10, i1* %2, align 1
  %11 = load i1, i1* %2, align 1
  br i1 %11, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %3, align 4
  %12 = load i32, i32* %3, align 4
  ret i32 %12

if.end:                                           ; preds = %entry
  %13 = load i32, i32* %input, align 4
  %14 = load i32, i32* %input, align 4
  store i32 1, i32* %4, align 4
  %15 = load i32, i32* %4, align 4
  %16 = sub i32 %14, %15
  store i32 %16, i32* %5, align 4
  %17 = load i32, i32* %5, align 4
  %18 = call i32 @"faculty(int)"(i32 %17)
  store i32 %18, i32* %6, align 4
  %19 = load i32, i32* %6, align 4
  %20 = mul i32 %13, %19
  store i32 %20, i32* %7, align 4
  %21 = load i32, i32* %7, align 4
  store i32 %21, i32* %result, align 4
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %0 = alloca i32, align 4
  %faculty = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 10, i32* %0, align 4
  %2 = load i32, i32* %0, align 4
  store i32 %2, i32* %input, align 4
  %3 = load i32, i32* %input, align 4
  %4 = call i32 @"faculty(int)"(i32 %3)
  store i32 %4, i32* %1, align 4
  %5 = load i32, i32* %1, align 4
  store i32 %5, i32* %faculty, align 4
  %6 = load i32, i32* %input, align 4
  %7 = load i32, i32* %faculty, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %6, i32 %7)
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

attributes #0 = { nounwind }
