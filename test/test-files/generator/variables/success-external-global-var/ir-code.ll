; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [16 x i8] c"Global var: %s\0A\00", align 1
@GLOBAL = external global i8*

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = load i8*, i8** @GLOBAL, align 8
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0), i8* %0)
  %2 = load i32, i32* %result, align 4
  ret i32 %2
}

declare i32 @printf(i8*, ...)
