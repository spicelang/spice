; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [14 x i8] c"Result 1: %d\0A\00", align 1
@1 = private unnamed_addr constant [14 x i8] c"Result 2: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  %13 = load i32, i32* %0, align 4
  store i32 %13, i32* %i, align 4
  store i16 2, i16* %1, align 2
  %14 = load i16, i16* %1, align 2
  %15 = load i32, i32* %i, align 4
  %16 = sext i16 %14 to i32
  %17 = add i32 %15, %16
  store i32 %17, i32* %i, align 4
  store i16 2, i16* %2, align 2
  %18 = load i16, i16* %2, align 2
  %19 = load i32, i32* %i, align 4
  %20 = sext i16 %18 to i32
  %21 = mul i32 %19, %20
  store i32 %21, i32* %i, align 4
  store i16 2, i16* %3, align 2
  %22 = load i16, i16* %3, align 2
  %23 = load i32, i32* %i, align 4
  %24 = sext i16 %22 to i32
  %25 = sdiv i32 %23, %24
  store i32 %25, i32* %i, align 4
  store i16 2, i16* %4, align 2
  %26 = load i16, i16* %4, align 2
  %27 = load i32, i32* %i, align 4
  %28 = sext i16 %26 to i32
  %29 = sub i32 %27, %28
  store i32 %29, i32* %i, align 4
  %30 = load i32, i32* %i, align 4
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0, i32 0, i32 0), i32 %30)
  store i64 223372036854775807, i64* %5, align 8
  %32 = load i64, i64* %5, align 8
  %33 = load i32, i32* %i, align 4
  %34 = sext i32 %33 to i64
  %35 = add i64 %34, %32
  store i64 %35, i64* %6, align 8
  store i64 2, i64* %7, align 8
  %36 = load i64, i64* %7, align 8
  %37 = load i32, i32* %i, align 4
  %38 = sext i32 %37 to i64
  %39 = sdiv i64 %38, %36
  store i64 %39, i64* %8, align 8
  store i64 2, i64* %9, align 8
  %40 = load i64, i64* %9, align 8
  %41 = load i32, i32* %i, align 4
  %42 = sext i32 %41 to i64
  %43 = mul i64 %42, %40
  store i64 %43, i64* %10, align 8
  store i64 223372036854775807, i64* %11, align 8
  %44 = load i64, i64* %11, align 8
  %45 = load i32, i32* %i, align 4
  %46 = sext i32 %45 to i64
  %47 = sub i64 %46, %44
  store i64 %47, i64* %12, align 8
  %48 = load i32, i32* %i, align 4
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @1, i32 0, i32 0), i32 %48)
  %50 = load i32, i32* %result, align 4
  ret i32 %50
}
