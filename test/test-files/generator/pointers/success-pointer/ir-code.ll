; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Pizza\00", align 1
@1 = private unnamed_addr constant [31 x i8] c"Pointer address: %p, value: %s\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"Burger\00", align 1
@3 = private unnamed_addr constant [19 x i8] c"Restored value: %s\00", align 1
@4 = private unnamed_addr constant [28 x i8] c"Restored value address: %p\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %food = alloca i8*, align 8
  %0 = alloca i8*, align 8
  %ptr = alloca i8**, align 8
  %1 = alloca i8**, align 8
  %restoredFood = alloca i8*, align 8
  %2 = alloca i8**, align 8
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  store i8** %0, i8*** %1, align 8
  %3 = load i8**, i8*** %1, align 8
  %4 = load i8**, i8*** %1, align 8
  %5 = load i8*, i8** %4, align 8
  store i8* %5, i8** %4, align 8
  %6 = load i8*, i8** %4, align 8
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @1, i32 0, i32 0), i8** %3, i8* %6)
  %8 = load i8**, i8*** %1, align 8
  %9 = load i8*, i8** %8, align 8
  store i8* %9, i8** %8, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @2, i32 0, i32 0), i8** %8, align 8
  %10 = load i8**, i8*** %1, align 8
  %11 = load i8*, i8** %10, align 8
  store i8* %11, i8** %10, align 8
  %12 = load i8*, i8** %10, align 8
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @3, i32 0, i32 0), i8* %12)
  store i8** %10, i8*** %2, align 8
  %14 = load i8**, i8*** %2, align 8
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @4, i32 0, i32 0), i8** %14)
  %16 = load i32, i32* %result, align 4
  ret i32 %16
}

declare i32 @printf(i8*, ...)
