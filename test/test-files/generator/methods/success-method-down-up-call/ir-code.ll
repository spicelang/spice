; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__char__TestStruct__char_int = type { i8, i32 }

@0 = private unnamed_addr constant [10 x i8] c"Test: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca %_s__char__TestStruct__char_int, align 8
  %1 = getelementptr inbounds %_s__char__TestStruct__char_int, ptr %0, i32 0, i32 0
  store i8 97, ptr %1, align 1
  %2 = getelementptr inbounds %_s__char__TestStruct__char_int, ptr %0, i32 0, i32 1
  store i32 1, ptr %2, align 4
  call void @_mp__TestStruct_char__printTest(ptr %0)
  %3 = alloca i1, align 1
  store i1 true, ptr %3, align 1
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

define internal void @_mp__TestStruct_char__printTest(ptr %0) {
entry.l13:
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %1 = load ptr, ptr %this, align 8
  %2 = call i32 @_mf__TestStruct_char__getTest(ptr %1)
  %3 = alloca i32, align 4
  store i32 %2, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0, i32 %4)
  ret void
}

declare i32 @printf(ptr, ...)

define internal i32 @_mf__TestStruct_char__getTest(ptr %0) {
entry.l17:
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %result = alloca i32, align 4
  %1 = load ptr, ptr %this, align 8
  %2 = getelementptr inbounds %_s__char__TestStruct__char_int, ptr %1, i32 0, i32 1
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 1
  %5 = alloca i1, align 1
  store i1 %4, ptr %5, align 1
  %6 = load i1, ptr %5, align 1
  br i1 %6, label %if.then.l18, label %if.end.l18

if.then.l18:                                      ; preds = %entry.l17
  %7 = load ptr, ptr %this, align 8
  %8 = getelementptr inbounds %_s__char__TestStruct__char_int, ptr %7, i32 0, i32 1
  %9 = load i32, ptr %8, align 4
  %10 = add i32 %9, 1
  store i32 %10, ptr %8, align 4
  %11 = alloca i32, align 4
  store i32 %9, ptr %11, align 4
  %12 = load ptr, ptr %this, align 8
  call void @_mp__TestStruct_char__printTest(ptr %12)
  %13 = alloca i1, align 1
  store i1 true, ptr %13, align 1
  br label %if.end.l18

if.end.l18:                                       ; preds = %if.then.l18, %entry.l17
  %14 = load ptr, ptr %this, align 8
  %15 = getelementptr inbounds %_s__char__TestStruct__char_int, ptr %14, i32 0, i32 1
  %16 = load i32, ptr %15, align 4
  ret i32 %16
}
