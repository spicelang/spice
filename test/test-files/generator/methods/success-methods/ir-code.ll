; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Letter__string = type { i8* }

@0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1

define internal i8* @_mf__Letter__getContent(%_s__Letter__string* %0) {
entry:
  %this = alloca %_s__Letter__string*, align 8
  %result = alloca i8*, align 8
  store %_s__Letter__string* %0, %_s__Letter__string** %this, align 8
  %1 = load %_s__Letter__string*, %_s__Letter__string** %this, align 8
  %2 = load %_s__Letter__string*, %_s__Letter__string** %this, align 8
  %3 = getelementptr inbounds %_s__Letter__string, %_s__Letter__string* %2, i32 0, i32 0
  %4 = load i8*, i8** %3, align 8
  ret i8* %4
}

define internal void @_mp__Letter__setContent__string(%_s__Letter__string* %0, i8* %1) {
entry:
  %this = alloca %_s__Letter__string*, align 8
  %text = alloca i8*, align 8
  store %_s__Letter__string* %0, %_s__Letter__string** %this, align 8
  store i8* %1, i8** %text, align 8
  %2 = load i8*, i8** %text, align 8
  %3 = load %_s__Letter__string*, %_s__Letter__string** %this, align 8
  %4 = load %_s__Letter__string*, %_s__Letter__string** %this, align 8
  %5 = getelementptr inbounds %_s__Letter__string, %_s__Letter__string* %4, i32 0, i32 0
  store i8* %2, i8** %5, align 8
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %letter = alloca %_s__Letter__string, align 8
  %0 = alloca %_s__Letter__string, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i1, align 1
  %4 = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %5 = load i8*, i8** %1, align 8
  %6 = getelementptr inbounds %_s__Letter__string, %_s__Letter__string* %0, i32 0, i32 0
  store i8* %5, i8** %6, align 8
  %7 = load %_s__Letter__string, %_s__Letter__string* %0, align 8
  store %_s__Letter__string %7, %_s__Letter__string* %letter, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %8 = load i8*, i8** %2, align 8
  call void @_mp__Letter__setContent__string(%_s__Letter__string* %letter, i8* %8)
  store i1 true, i1* %3, align 1
  %9 = call i8* @_mf__Letter__getContent(%_s__Letter__string* %letter)
  store i8* %9, i8** %4, align 8
  %10 = load i8*, i8** %4, align 8
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @2, i32 0, i32 0), i8* %10)
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

declare i32 @printf(i8*, ...)
