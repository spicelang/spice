; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Test: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca { i8, i32 }, align 8
  %1 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  %2 = getelementptr inbounds { i8, i32 }, { i8, i32 }* %0, i32 0, i32 0
  store i8 97, i8* %2, align 1
  %3 = getelementptr inbounds { i8, i32 }, { i8, i32 }* %0, i32 0, i32 1
  store i32 1, i32* %3, align 4
  call void @_mp__TestStruct_char__printTest({ i8, i32 }* %0)
  store i1 true, i1* %1, align 1
  %4 = load i32, i32* %result, align 4
  ret i32 %4
}

define internal void @_mp__TestStruct_char__printTest({ i8, i32 }* %0) {
entry.l13:
  %this = alloca { i8, i32 }*, align 8
  %1 = alloca i32, align 4
  store { i8, i32 }* %0, { i8, i32 }** %this, align 8
  %2 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %3 = call i32 @_mf__TestStruct_char__getTest({ i8, i32 }* %2)
  store i32 %3, i32* %1, align 4
  %4 = load i32, i32* %1, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %4)
  ret void
}

declare i32 @printf(i8*, ...)

define internal i32 @_mf__TestStruct_char__getTest({ i8, i32 }* %0) {
entry.l17:
  %this = alloca { i8, i32 }*, align 8
  %result = alloca i32, align 4
  %1 = alloca i1, align 1
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  store { i8, i32 }* %0, { i8, i32 }** %this, align 8
  %4 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %5 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %6 = getelementptr inbounds { i8, i32 }, { i8, i32 }* %5, i32 0, i32 1
  %7 = load i32, i32* %6, align 4
  %8 = icmp eq i32 %7, 1
  store i1 %8, i1* %1, align 1
  %9 = load i1, i1* %1, align 1
  br i1 %9, label %if.then.l18, label %if.end.l18

if.then.l18:                                      ; preds = %entry.l17
  %10 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %11 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %12 = getelementptr inbounds { i8, i32 }, { i8, i32 }* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 4
  %14 = add i32 %13, 1
  store i32 %14, i32* %12, align 4
  store i32 %13, i32* %2, align 4
  %15 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  call void @_mp__TestStruct_char__printTest({ i8, i32 }* %15)
  store i1 true, i1* %3, align 1
  br label %if.end.l18

if.end.l18:                                       ; preds = %if.then.l18, %entry.l17
  %16 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %17 = load { i8, i32 }*, { i8, i32 }** %this, align 8
  %18 = getelementptr inbounds { i8, i32 }, { i8, i32 }* %17, i32 0, i32 1
  %19 = load i32, i32* %18, align 4
  ret i32 %19
}
