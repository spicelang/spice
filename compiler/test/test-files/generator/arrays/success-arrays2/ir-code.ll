; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@intArray = dso_local constant [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512]
@0 = private unnamed_addr constant [17 x i8] c"intArray[3]: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"intArray[7]: %d\0A\00", align 1
@2 = private unnamed_addr constant [17 x i8] c"intArray[9]: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [10 x i32], align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  store i32 2, i32* %1, align 4
  store i32 4, i32* %2, align 4
  store i32 8, i32* %3, align 4
  store i32 16, i32* %4, align 4
  store i32 32, i32* %5, align 4
  store i32 64, i32* %6, align 4
  store i32 128, i32* %7, align 4
  store i32 256, i32* %8, align 4
  store i32 512, i32* %9, align 4
  %13 = load [10 x i32], [10 x i32]* @intArray, align 4
  store [10 x i32] %13, [10 x i32]* %intArray, align 4
  %14 = load [10 x i32], [10 x i32]* %intArray, align 4
  store i32 3, i32* %10, align 4
  %15 = load i32, i32* %10, align 4
  %16 = getelementptr inbounds [10 x i32], [10 x i32]* %intArray, i32 0, i32 %15
  %17 = load i32, i32* %16, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), i32 %17)
  %19 = load [10 x i32], [10 x i32]* %intArray, align 4
  store i32 7, i32* %11, align 4
  %20 = load i32, i32* %11, align 4
  %21 = getelementptr inbounds [10 x i32], [10 x i32]* %intArray, i32 0, i32 %20
  %22 = load i32, i32* %21, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %22)
  %24 = load [10 x i32], [10 x i32]* %intArray, align 4
  store i32 9, i32* %12, align 4
  %25 = load i32, i32* %12, align 4
  %26 = getelementptr inbounds [10 x i32], [10 x i32]* %intArray, i32 0, i32 %25
  %27 = load i32, i32* %26, align 4
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @2, i32 0, i32 0), i32 %27)
  %29 = load i32, i32* %result, align 4
  ret i32 %29
}
