; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__TestStruct__long = type { i64 }

@anon.struct.0 = private unnamed_addr constant %__TestStruct__long { i64 123 }
@anon.string.0 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'output.test == 125l' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'ts.test == 125l' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

define private ptr @_f__void__TestStructref__op.plusplus.post__TestStructref(ptr %0) {
  %result = alloca ptr, align 8
  %ts = alloca ptr, align 8
  store ptr %0, ptr %ts, align 8
  %2 = load ptr, ptr %ts, align 8
  %test_addr = getelementptr inbounds %__TestStruct__long, ptr %2, i32 0, i32 0
  %3 = load i64, ptr %test_addr, align 8
  %4 = add i64 %3, 1
  store i64 %4, ptr %test_addr, align 8
  %5 = load ptr, ptr %ts, align 8
  ret ptr %5
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %ts = alloca %__TestStruct__long, align 8
  %output = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %ts, ptr @anon.struct.0, i64 8, i1 false)
  store %__TestStruct__long { i64 123 }, ptr %ts, align 8
  %1 = load %__TestStruct__long, ptr %ts, align 8
  %2 = call ptr @_f__void__TestStructref__op.plusplus.post__TestStructref(ptr %ts)
  store ptr %2, ptr %output, align 8
  %test_addr = getelementptr inbounds %__TestStruct__long, ptr %ts, i32 0, i32 0
  %3 = load i64, ptr %test_addr, align 8
  %4 = add i64 %3, 1
  store i64 %4, ptr %test_addr, align 8
  %5 = load ptr, ptr %output, align 8
  %test_addr1 = getelementptr inbounds %__TestStruct__long, ptr %5, i32 0, i32 0
  %6 = load i64, ptr %test_addr1, align 8
  %7 = icmp eq i64 %6, 125
  br i1 %7, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %0
  %test_addr2 = getelementptr inbounds %__TestStruct__long, ptr %ts, i32 0, i32 0
  %9 = load i64, ptr %test_addr2, align 8
  %10 = icmp eq i64 %9, 125
  br i1 %10, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %12 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!0 = !{!"branch_weights", i32 2000, i32 1}
