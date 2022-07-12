; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [16 x i8] c"Condition true\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Condition false\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %condition = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i1 true, i1* %0, align 1
  %1 = load i1, i1* %0, align 1
  br i1 %1, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0))
  store i32 0, i32* %condition, align 4
  %3 = load i32, i32* %condition, align 4
  ret i32 %3

if.end:                                           ; preds = %entry
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0))
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}

declare i32 @printf(i8*, ...)
