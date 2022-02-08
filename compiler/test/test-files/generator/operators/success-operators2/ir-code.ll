; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [14 x i8] c"Result 1: %d\0A\00", align 1

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
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  %5 = load i32, i32* %0, align 4
  store i32 %5, i32* %i, align 4
  store i16 2, i16* %1, align 2
  %6 = load i16, i16* %1, align 2
  %7 = load i32, i32* %i, align 4
  %8 = sext i16 %6 to i32
  %9 = add i32 %7, %8
  store i32 %9, i32* %i, align 4
  store i16 2, i16* %2, align 2
  %10 = load i16, i16* %2, align 2
  %11 = load i32, i32* %i, align 4
  %12 = sext i16 %10 to i32
  %13 = mul i32 %11, %12
  store i32 %13, i32* %i, align 4
  store i16 2, i16* %3, align 2
  %14 = load i16, i16* %3, align 2
  %15 = load i32, i32* %i, align 4
  %16 = sext i16 %14 to i32
  %17 = sdiv i32 %15, %16
  store i32 %17, i32* %i, align 4
  store i16 2, i16* %4, align 2
  %18 = load i16, i16* %4, align 2
  %19 = load i32, i32* %i, align 4
  %20 = sext i16 %18 to i32
  %21 = sub i32 %19, %20
  store i32 %21, i32* %i, align 4
  %22 = load i32, i32* %i, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0, i32 0, i32 0), i32 %22)
  %24 = load i32, i32* %result, align 4
  ret i32 %24
}
