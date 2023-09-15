; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Vector = type { i1, ptr }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Destructor called!\00", align 1
@anon.string.1 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'this.field1 == true' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@anon.string.3 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 'this.field2 == \22Test\22' evaluated to false.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %vec = alloca %struct.Vector, align 8
  store i32 0, ptr %result, align 4
  store %struct.Vector { i1 true, ptr @anon.string.0 }, ptr %vec, align 8
  %field1_addr = getelementptr inbounds %struct.Vector, ptr %vec, i32 0, i32 0
  %1 = load i1, ptr %field1_addr, align 1
  %2 = zext i1 %1 to i32
  %field2_addr = getelementptr inbounds %struct.Vector, ptr %vec, i32 0, i32 1
  %3 = load ptr, ptr %field2_addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2, ptr %3)
  call void @_ZN6Vector4dtorEv(ptr %vec)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

define private void @_ZN6Vector4dtorEv(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  %2 = alloca i1, align 1
  store ptr %0, ptr %this, align 8
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  %4 = load ptr, ptr %this, align 8
  %field1_addr = getelementptr inbounds %struct.Vector, ptr %4, i32 0, i32 0
  store i1 true, ptr %2, align 1
  %5 = call i32 @memcmp(ptr %field1_addr, ptr %2, i64 0)
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %1
  %7 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L13:                                  ; preds = %1
  %8 = load ptr, ptr %this, align 8
  %field2_addr = getelementptr inbounds %struct.Vector, ptr %8, i32 0, i32 1
  %9 = load ptr, ptr %field2_addr, align 8
  %10 = call i1 @_Z10isRawEqualPcPc(ptr %9, ptr @anon.string.2)
  br i1 %10, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %11 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  ret void
}

; Function Attrs: nounwind
declare i32 @memcmp(ptr, ptr, i64) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

declare i1 @_Z10isRawEqualPcPc(ptr, ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
attributes #3 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
