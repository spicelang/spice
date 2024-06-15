; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, i64 }
%struct.TestPacked = type <{ i32, i64 }>

@anon.string.0 = private unnamed_addr constant [78 x i8] c"Assertion failed: Condition 'sizeof(type Test) == 16 * 8' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'sizeof(t) == 16 * 8' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 't.f1 == -2147483647' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [79 x i8] c"Assertion failed: Condition 't.f2 == 9223372036854775807l' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [84 x i8] c"Assertion failed: Condition 'sizeof(type TestPacked) == 12 * 8' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'sizeof(tp) == 12 * 8' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'tp.f1 == -2147483647' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [80 x i8] c"Assertion failed: Condition 'tp.f2 == 9223372036854775807l' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  %tp = alloca %struct.TestPacked, align 8
  store i32 0, ptr %result, align 4
  store %struct.Test zeroinitializer, ptr %t, align 8
  %f1_addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  store i32 -2147483647, ptr %f1_addr, align 4
  %f2_addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  store i64 9223372036854775807, ptr %f2_addr, align 8
  br i1 true, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %0
  %1 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %0
  br i1 true, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %2 = call i32 (ptr, ...) @printf(ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %f1_addr1 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %3 = load i32, ptr %f1_addr1, align 4
  %4 = icmp eq i32 %3, -2147483647
  br i1 %4, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %5 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  %f2_addr2 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  %6 = load i64, ptr %f2_addr2, align 8
  %7 = icmp eq i64 %6, 9223372036854775807
  br i1 %7, label %assert.exit.L19, label %assert.then.L19, !prof !0

assert.then.L19:                                  ; preds = %assert.exit.L18
  %8 = call i32 (ptr, ...) @printf(ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L18
  store %struct.TestPacked zeroinitializer, ptr %tp, align 1
  %f1_addr3 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 0
  store i32 -2147483647, ptr %f1_addr3, align 4
  %f2_addr4 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 1
  store i64 9223372036854775807, ptr %f2_addr4, align 8
  br i1 true, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L19
  %9 = call i32 (ptr, ...) @printf(ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L19
  br i1 true, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L25
  %10 = call i32 (ptr, ...) @printf(ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L25
  %f1_addr5 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 0
  %11 = load i32, ptr %f1_addr5, align 4
  %12 = icmp eq i32 %11, -2147483647
  br i1 %12, label %assert.exit.L27, label %assert.then.L27, !prof !0

assert.then.L27:                                  ; preds = %assert.exit.L26
  %13 = call i32 (ptr, ...) @printf(ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L26
  %f2_addr6 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 1
  %14 = load i64, ptr %f2_addr6, align 8
  %15 = icmp eq i64 %14, 9223372036854775807
  br i1 %15, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L27
  %16 = call i32 (ptr, ...) @printf(ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L27
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
