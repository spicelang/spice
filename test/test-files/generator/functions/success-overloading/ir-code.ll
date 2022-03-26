; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [12 x i8] c"Test func 1\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Test func 2\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 1

; Function Attrs: nounwind
define internal i32 @_f_int_testFunc() #0 {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0))
  store i32 1, i32* %0, align 4
  %2 = load i32, i32* %0, align 4
  ret i32 %2
}

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define internal i32 @_f_int_testFunc_string(i8* %0) #0 {
entry:
  %param = alloca i8*, align 8
  %result = alloca i32, align 4
  %1 = alloca i32, align 4
  store i8* %0, i8** %param, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0))
  store i32 2, i32* %1, align 4
  %3 = load i32, i32* %1, align 4
  ret i32 %3
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %res = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %1 = call i32 @_f_int_testFunc()
  store i32 %1, i32* %0, align 4
  %2 = load i32, i32* %0, align 4
  store i32 %2, i32* %res, align 4
  %3 = load i32, i32* %res, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i32 %3)
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}

attributes #0 = { nounwind }
