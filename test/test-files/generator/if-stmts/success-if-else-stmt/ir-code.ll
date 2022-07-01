; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 1
@2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"Else\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i1 false, i1* %0, align 1
  %3 = load i1, i1* %0, align 1
  br i1 %3, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0))
  br label %if.end6

if.else:                                          ; preds = %entry
  store i1 false, i1* %1, align 1
  %5 = load i1, i1* %1, align 1
  br i1 %5, label %if.then1, label %if.else2

if.then1:                                         ; preds = %if.else
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0))
  br label %if.end5

if.else2:                                         ; preds = %if.else
  store i1 false, i1* %2, align 1
  %7 = load i1, i1* %2, align 1
  br i1 %7, label %if.then3, label %if.else4

if.then3:                                         ; preds = %if.else2
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @2, i32 0, i32 0))
  br label %if.end

if.else4:                                         ; preds = %if.else2
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @3, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.else4, %if.then3
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then1
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %if.then
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}

declare i32 @printf(i8*, ...)
