; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [15 x i8] c"The age is: %d\00", align 1

define internal i32 @_f__void__getAge() {
entry:
  %result = alloca i32, align 4
  %i = alloca i1, align 1
  %0 = alloca i32, align 4
  store i1 false, i1* %i, align 1
  br i1 true, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 20, i32* %result, align 4
  %1 = load i32, i32* %result, align 4
  ret i32 %1

if.else:                                          ; preds = %entry
  store i1 false, i1* %i, align 1
  %2 = load i1, i1* %i, align 1
  br i1 %2, label %if.then1, label %if.end

if.then1:                                         ; preds = %if.else
  store i32 19, i32* %result, align 4
  br label %if.end

if.end:                                           ; preds = %if.then1, %if.else
  br label %if.end2

if.end2:                                          ; preds = %if.end
  store i32 15, i32* %0, align 4
  %3 = load i32, i32* %0, align 4
  ret i32 %3
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %1 = call i32 @_f__void__getAge()
  store i32 %1, i32* %0, align 4
  %2 = load i32, i32* %0, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0), i32 %2)
  %4 = load i32, i32* %result, align 4
  ret i32 %4
}

declare i32 @printf(i8*, ...)
