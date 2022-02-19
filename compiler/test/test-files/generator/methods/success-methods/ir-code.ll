; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%Letter = type { i8* }

@0 = private unnamed_addr constant [11 x i8] c"No content\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define internal i8* @"getContent()"(%Letter* %0) #0 {
entry:
  %this = alloca %Letter*, align 8
  %result = alloca i8*, align 8
  store %Letter* %0, %Letter** %this, align 8
  %1 = load %Letter*, %Letter** %this, align 8
  %2 = load %Letter*, %Letter** %this, align 8
  %3 = getelementptr inbounds %Letter, %Letter* %2, i32 0, i32 0
  %4 = load i8*, i8** %3, align 8
  ret i8* %4
}

; Function Attrs: nounwind
define internal void @"setContent(string)"(%Letter* %0, i8* %1) #0 {
entry:
  %this = alloca %Letter*, align 8
  %text = alloca i8*, align 8
  store %Letter* %0, %Letter** %this, align 8
  store i8* %1, i8** %text, align 8
  %2 = load i8*, i8** %text, align 8
  %3 = load %Letter*, %Letter** %this, align 8
  %4 = load %Letter*, %Letter** %this, align 8
  %5 = getelementptr inbounds %Letter, %Letter* %4, i32 0, i32 0
  store i8* %2, i8** %5, align 8
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %letter = alloca %Letter, align 8
  %0 = alloca %Letter, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i8*, align 8
  %3 = alloca i1, align 1
  %4 = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %5 = load i8*, i8** %1, align 8
  %6 = getelementptr inbounds %Letter, %Letter* %0, i32 0, i32 0
  store i8* %5, i8** %6, align 8
  %7 = load %Letter, %Letter* %0, align 8
  store %Letter %7, %Letter* %letter, align 8
  %8 = load %Letter, %Letter* %letter, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @1, i32 0, i32 0), i8** %2, align 8
  %9 = load i8*, i8** %2, align 8
  call void @"setContent(string)"(%Letter* %letter, i8* %9)
  store i1 true, i1* %3, align 1
  %10 = load %Letter, %Letter* %letter, align 8
  %11 = call i8* @"getContent()"(%Letter* %letter)
  store i8* %11, i8** %4, align 8
  %12 = load i8*, i8** %4, align 8
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @2, i32 0, i32 0), i8* %12)
  %14 = load i32, i32* %result, align 4
  ret i32 %14
}

attributes #0 = { nounwind }
