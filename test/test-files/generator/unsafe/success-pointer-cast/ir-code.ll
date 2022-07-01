; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %l = alloca i64, align 8
  %lPtr = alloca i64*, align 8
  %0 = alloca i64*, align 8
  %iPtr = alloca i32*, align 8
  %1 = alloca i32*, align 8
  store i32 0, i32* %result, align 4
  store i64 1234, i64* %l, align 8
  %2 = load i64, i64* %l, align 8
  store i64* %l, i64** %0, align 8
  store i64* %l, i64** %0, align 8
  %3 = load i64*, i64** %0, align 8
  store i64* %3, i64** %lPtr, align 8
  %4 = load i64*, i64** %lPtr, align 8
  %5 = bitcast i64* %4 to i32*
  store i32* %5, i32** %1, align 8
  %6 = load i32*, i32** %1, align 8
  store i32* %6, i32** %iPtr, align 8
  %7 = load i32*, i32** %iPtr, align 8
  %8 = load i32, i32* %7, align 4
  store i32 %8, i32* %7, align 4
  %9 = load i32, i32* %7, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0), i32 %9)
  %11 = load i32, i32* %result, align 4
  ret i32 %11
}

declare i32 @printf(i8*, ...)
