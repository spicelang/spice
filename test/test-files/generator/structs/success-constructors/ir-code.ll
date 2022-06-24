; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Vector__bool_string = type { i1, i8* }

@0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 1
@1 = private unnamed_addr constant [15 x i8] c"Fields: %d, %s\00", align 1

define internal void @_mp__Vector__ctor(%_s__Vector__bool_string* %0) {
entry:
  %this = alloca %_s__Vector__bool_string*, align 8
  %1 = alloca i1, align 1
  %2 = alloca i8*, align 8
  store %_s__Vector__bool_string* %0, %_s__Vector__bool_string** %this, align 8
  store i1 false, i1* %1, align 1
  %3 = load i1, i1* %1, align 1
  %4 = load %_s__Vector__bool_string*, %_s__Vector__bool_string** %this, align 8
  %5 = load %_s__Vector__bool_string*, %_s__Vector__bool_string** %this, align 8
  %6 = getelementptr inbounds %_s__Vector__bool_string, %_s__Vector__bool_string* %5, i32 0, i32 0
  store i1 %3, i1* %6, align 1
  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0), i8** %2, align 8
  %7 = load i8*, i8** %2, align 8
  %8 = load %_s__Vector__bool_string*, %_s__Vector__bool_string** %this, align 8
  %9 = load %_s__Vector__bool_string*, %_s__Vector__bool_string** %this, align 8
  %10 = getelementptr inbounds %_s__Vector__bool_string, %_s__Vector__bool_string* %9, i32 0, i32 1
  store i8* %7, i8** %10, align 8
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %vec = alloca %_s__Vector__bool_string, align 8
  store i32 0, i32* %result, align 4
  call void @_mp__Vector__ctor(%_s__Vector__bool_string* %vec)
  %0 = load %_s__Vector__bool_string, %_s__Vector__bool_string* %vec, align 8
  store %_s__Vector__bool_string %0, %_s__Vector__bool_string* %vec, align 8
  %1 = load %_s__Vector__bool_string, %_s__Vector__bool_string* %vec, align 8
  %2 = getelementptr inbounds %_s__Vector__bool_string, %_s__Vector__bool_string* %vec, i32 0, i32 0
  %3 = load i1, i1* %2, align 1
  %4 = zext i1 %3 to i32
  %5 = load %_s__Vector__bool_string, %_s__Vector__bool_string* %vec, align 8
  %6 = getelementptr inbounds %_s__Vector__bool_string, %_s__Vector__bool_string* %vec, i32 0, i32 1
  %7 = load i8*, i8** %6, align 8
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @1, i32 0, i32 0), i32 %4, i8* %7)
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

declare i32 @printf(i8*, ...)
