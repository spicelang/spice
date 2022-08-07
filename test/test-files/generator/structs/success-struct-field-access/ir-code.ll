; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [5 x i8] c"John\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"Doe\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"John's age: %d\00", align 1

define i32 @main() {
entry.l7:
  %result = alloca i32, align 4
  %0 = alloca { i8*, i8*, i32 }, align 8
  store i32 0, i32* %result, align 4
  %1 = getelementptr inbounds { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %2 = getelementptr inbounds { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, i32 0, i32 1
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %3 = getelementptr inbounds { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, i32 0, i32 2
  store i32 46, i32* %3, align 4
  %4 = load { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, align 8
  %5 = getelementptr inbounds { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, i32 0, i32 2
  store i32 47, i32* %5, align 4
  %6 = load { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, align 8
  %7 = getelementptr inbounds { i8*, i8*, i32 }, { i8*, i8*, i32 }* %0, i32 0, i32 2
  %8 = load i32, i32* %7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @2, i32 0, i32 0), i32 %8)
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}

declare i32 @printf(i8*, ...)
