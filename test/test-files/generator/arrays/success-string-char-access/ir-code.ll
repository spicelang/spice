; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Char: %c\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %test = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %test, align 8
  %0 = load i8*, i8** %test, align 8
  %1 = getelementptr inbounds i8, i8* %0, i32 2
  %2 = load i8, i8* %1, align 1
  %3 = sext i8 %2 to i32
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0), i32 %3)
  %5 = load i32, i32* %result, align 4
  ret i32 %5
}

declare i32 @printf(i8*, ...)
