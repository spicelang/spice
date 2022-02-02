; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Argc: %d\0A\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Argv no. 0: %s\0A\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Argv no. 1: %s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %argc = alloca i32, align 4
  %argv = alloca i8**, align 8
  %result = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  store i32 %0, i32* %argc, align 4
  store i8** %1, i8*** %argv, align 8
  store i32 0, i32* %result, align 4
  %6 = load i32, i32* %argc, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %6)
  %8 = load i8**, i8*** %argv, align 8
  store i32 0, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  %10 = getelementptr inbounds i8*, i8** %8, i32 %9
  %11 = load i8*, i8** %10, align 8
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @1, i32 0, i32 0), i8* %11)
  store i32 1, i32* %3, align 4
  %13 = load i32, i32* %argc, align 4
  %14 = load i32, i32* %3, align 4
  %15 = icmp sgt i32 %13, %14
  store i1 %15, i1* %4, align 1
  %16 = load i1, i1* %4, align 1
  br i1 %16, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %17 = load i8**, i8*** %argv, align 8
  store i32 1, i32* %5, align 4
  %18 = load i32, i32* %5, align 4
  %19 = getelementptr inbounds i8*, i8** %17, i32 %18
  %20 = load i8*, i8** %19, align 8
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %20)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %22 = load i32, i32* %result, align 4
  ret i32 %22
}
