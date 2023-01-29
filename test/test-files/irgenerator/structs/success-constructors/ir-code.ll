; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Vector__bool_string = type { i1, ptr }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 1
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [16 x i8] c"Another message\00", align 1
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Message: %s\0A\00", align 1

define private void @_mp__Vector__void__ctor(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr @anon.string.0, ptr %msg, align 8
  %2 = load ptr, ptr %this, align 8
  %field1 = getelementptr inbounds %__Vector__bool_string, ptr %2, i32 0, i32 0
  store i1 false, ptr %field1, align 1
  %3 = load ptr, ptr %this, align 8
  %field2 = getelementptr inbounds %__Vector__bool_string, ptr %3, i32 0, i32 1
  %4 = load ptr, ptr %msg, align 8
  store ptr %4, ptr %field2, align 8
  ret void
}

define private void @_mp__Vector__void__ctor__string(ptr noundef nonnull %0, ptr %1) {
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %msg, align 8
  %3 = load ptr, ptr %this, align 8
  %field1 = getelementptr inbounds %__Vector__bool_string, ptr %3, i32 0, i32 0
  store i1 false, ptr %field1, align 1
  %4 = load ptr, ptr %this, align 8
  %field2 = getelementptr inbounds %__Vector__bool_string, ptr %4, i32 0, i32 1
  %5 = load ptr, ptr %msg, align 8
  store ptr %5, ptr %field2, align 8
  ret void
}

define private ptr @_mf__Vector__string__test(ptr noundef nonnull %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  ret ptr @anon.string.1
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__Vector__bool_string, align 8
  %vec = alloca %__Vector__bool_string, align 8
  %2 = alloca %__Vector__bool_string, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__Vector__void__ctor(ptr %1)
  %3 = load %__Vector__bool_string, ptr %1, align 8
  store %__Vector__bool_string %3, ptr %vec, align 8
  %field1 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 0
  %4 = load i1, ptr %field1, align 1
  %5 = zext i1 %4 to i32
  %field2 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 1
  %6 = load ptr, ptr %field2, align 8
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %5, ptr %6)
  call void @_mp__Vector__void__ctor__string(ptr %2, ptr @anon.string.2)
  call void @llvm.memcpy.p0.p0.i64(ptr %vec, ptr %2, i64 16, i1 false)
  %field11 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 0
  %8 = load i1, ptr %field11, align 1
  %9 = zext i1 %8 to i32
  %field22 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 1
  %10 = load ptr, ptr %field22, align 8
  %11 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %9, ptr %10)
  %12 = call ptr @_mf__Vector__string__test(ptr %vec)
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %12)
  %14 = load i32, ptr %result, align 4
  ret i32 %14
}

declare i32 @printf(ptr noundef, ...)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
