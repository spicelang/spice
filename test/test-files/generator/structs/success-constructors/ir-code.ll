; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%_s__Vector__bool_string = type { i1, ptr }

@0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 1
@1 = private unnamed_addr constant [15 x i8] c"Fields: %d, %s\00", align 1

define internal void @_mp__Vector__ctor(ptr %0) {
entry.l6:
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %1 = load ptr, ptr %this, align 8
  %2 = getelementptr inbounds %_s__Vector__bool_string, ptr %1, i32 0, i32 0
  store i1 false, ptr %2, align 1
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %_s__Vector__bool_string, ptr %3, i32 0, i32 1
  store ptr @0, ptr %4, align 8
  ret void
}

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %vec = alloca %_s__Vector__bool_string, align 8
  call void @_mp__Vector__ctor(ptr %vec)
  %0 = getelementptr inbounds %_s__Vector__bool_string, ptr %vec, i32 0, i32 0
  %1 = load i1, ptr %0, align 1
  %2 = zext i1 %1 to i32
  %3 = getelementptr inbounds %_s__Vector__bool_string, ptr %vec, i32 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = call i32 (ptr, ...) @printf(ptr @1, i32 %2, ptr %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr, ...)
