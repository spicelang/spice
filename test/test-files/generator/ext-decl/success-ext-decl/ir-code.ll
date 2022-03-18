; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@MODE_ALL_RWX = internal constant i32 511
@0 = private unnamed_addr constant [18 x i8] c"Creating dir ...\0A\00", align 1
@1 = private unnamed_addr constant [11 x i8] c"./test-dir\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"Deleting dir ...\0A\00", align 1
@3 = private unnamed_addr constant [11 x i8] c"./test-dir\00", align 1
@4 = private unnamed_addr constant [6 x i8] c"Done.\00", align 1

declare i32 @mkdir(i8*, i32)

declare i32 @rmdir(i8*)

declare void @free(i8*)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i8*, align 8
  %1 = alloca i32, align 4
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @0, i32 0, i32 0))
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @1, i32 0, i32 0), i8** %0, align 8
  %5 = load i8*, i8** %0, align 8
  %6 = load i32, i32* @MODE_ALL_RWX, align 4
  %7 = call i32 @mkdir(i8* %5, i32 %6)
  store i32 %7, i32* %1, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @2, i32 0, i32 0))
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @3, i32 0, i32 0), i8** %2, align 8
  %9 = load i8*, i8** %2, align 8
  %10 = call i32 @rmdir(i8* %9)
  store i32 %10, i32* %3, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @4, i32 0, i32 0))
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

declare i32 @printf(i8*, ...)
