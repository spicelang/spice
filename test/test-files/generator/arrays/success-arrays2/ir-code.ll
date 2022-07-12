; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@intArray = constant [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512]
@0 = private unnamed_addr constant [17 x i8] c"intArray[3]: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"intArray[7]: %d\0A\00", align 1
@2 = private unnamed_addr constant [17 x i8] c"intArray[9]: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [10 x i32], align 4
  store i32 0, i32* %result, align 4
  store [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512], [10 x i32]* %intArray, align 4
  %0 = load [10 x i32], [10 x i32]* %intArray, align 4
  %1 = getelementptr inbounds [10 x i32], [10 x i32]* %intArray, i32 0, i32 3
  %2 = load i32, i32* %1, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), i32 %2)
  %4 = load [10 x i32], [10 x i32]* %intArray, align 4
  %5 = getelementptr inbounds [10 x i32], [10 x i32]* %intArray, i32 0, i32 7
  %6 = load i32, i32* %5, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %6)
  %8 = load [10 x i32], [10 x i32]* %intArray, align 4
  %9 = getelementptr inbounds [10 x i32], [10 x i32]* %intArray, i32 0, i32 9
  %10 = load i32, i32* %9, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @2, i32 0, i32 0), i32 %10)
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

declare i32 @printf(i8*, ...)
