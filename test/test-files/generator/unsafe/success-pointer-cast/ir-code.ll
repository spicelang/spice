; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %l = alloca i64, align 8
  %0 = alloca i64, align 8
  %lPtr = alloca i64*, align 8
  %1 = alloca i64*, align 8
  %iPtr = alloca i32*, align 8
  %2 = alloca i32*, align 8
  store i32 0, i32* %result, align 4
  store i64 1234, i64* %0, align 8
  %3 = load i64, i64* %0, align 8
  store i64 %3, i64* %l, align 8
  %4 = load i64, i64* %l, align 8
  store i64* %l, i64** %1, align 8
  store i64* %l, i64** %1, align 8
  %5 = load i64*, i64** %1, align 8
  store i64* %5, i64** %lPtr, align 8
  %6 = load i64*, i64** %lPtr, align 8
  %7 = bitcast i64* %6 to i32*
  store i32* %7, i32** %2, align 8
  %8 = load i32*, i32** %2, align 8
  store i32* %8, i32** %iPtr, align 8
  %9 = load i32*, i32** %iPtr, align 8
  %10 = load i32, i32* %9, align 4
  store i32 %10, i32* %9, align 4
  %11 = load i32, i32* %9, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0), i32 %11)
  %13 = load i32, i32* %result, align 4
  ret i32 %13
}

declare i32 @printf(i8*, ...)
