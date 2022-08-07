; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 1
@1 = private unnamed_addr constant [15 x i8] c"Fields: %d, %s\00", align 1

define internal void @_mp__Vector__ctor({ i1, i8* }* %0) {
entry.l6:
  %this = alloca { i1, i8* }*, align 8
  store { i1, i8* }* %0, { i1, i8* }** %this, align 8
  %1 = load { i1, i8* }*, { i1, i8* }** %this, align 8
  %2 = load { i1, i8* }*, { i1, i8* }** %this, align 8
  %3 = getelementptr inbounds { i1, i8* }, { i1, i8* }* %2, i32 0, i32 0
  store i1 false, i1* %3, align 1
  %4 = load { i1, i8* }*, { i1, i8* }** %this, align 8
  %5 = load { i1, i8* }*, { i1, i8* }** %this, align 8
  %6 = getelementptr inbounds { i1, i8* }, { i1, i8* }* %5, i32 0, i32 1
  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0), i8** %6, align 8
  ret void
}

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %vec = alloca { i1, i8* }, align 8
  store i32 0, i32* %result, align 4
  call void @_mp__Vector__ctor({ i1, i8* }* %vec)
  %0 = load { i1, i8* }, { i1, i8* }* %vec, align 8
  %1 = getelementptr inbounds { i1, i8* }, { i1, i8* }* %vec, i32 0, i32 0
  %2 = load i1, i1* %1, align 1
  %3 = zext i1 %2 to i32
  %4 = load { i1, i8* }, { i1, i8* }* %vec, align 8
  %5 = getelementptr inbounds { i1, i8* }, { i1, i8* }* %vec, i32 0, i32 1
  %6 = load i8*, i8** %5, align 8
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @1, i32 0, i32 0), i32 %3, i8* %6)
  %8 = load i32, i32* %result, align 4
  ret i32 %8
}

declare i32 @printf(i8*, ...)
