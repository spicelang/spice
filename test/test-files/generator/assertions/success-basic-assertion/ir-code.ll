; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition '1!=1' evaluated to false.\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Unreachable\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  store i32 1, i32* %1, align 4
  %3 = load i32, i32* %0, align 4
  %4 = load i32, i32* %1, align 4
  %5 = icmp ne i32 %3, %4
  store i1 %5, i1* %2, align 1
  %6 = load i1, i1* %2, align 1
  br i1 %6, label %if.end, label %if.then

if.then:                                          ; preds = %entry
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([55 x i8], [55 x i8]* @0, i32 0, i32 0))
  call void @exit(i32 1)
  unreachable

if.end:                                           ; preds = %entry
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0))
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

declare i32 @printf(i8*, ...)

declare void @exit(i32)
