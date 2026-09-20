; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32, i64 }
%struct.TestPacked = type <{ i32, i64 }>

@anon.string.0 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 't.f1 == -2147483647' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [80 x i8] c"Assertion failed: Condition 't.f2 == 9223372036854775807l' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 'tp.f1 == -2147483647' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [81 x i8] c"Assertion failed: Condition 'tp.f2 == 9223372036854775807l' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %t = alloca %struct.Test, align 8
  %tp = alloca %struct.TestPacked, align 8
  store %struct.Test zeroinitializer, ptr %t, align 8
  %f1.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  store i32 -2147483647, ptr %f1.addr, align 4
  %f2.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  store i64 9223372036854775807, ptr %f2.addr, align 8
  %f1.addr1 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %1 = load i32, ptr %f1.addr1, align 4
  %2 = icmp eq i32 %1, -2147483647
  br i1 %2, label %assert.exit.L18, label %assert.then.L18, !prof !5

assert.then.L18:                                  ; preds = %0
  %3 = call ptr @__acrt_iob_func(i32 2)
  %4 = call i32 (ptr, ptr, ...) @fprintf(ptr %3, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %0
  %f2.addr2 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 1
  %5 = load i64, ptr %f2.addr2, align 8
  %6 = icmp eq i64 %5, 9223372036854775807
  br i1 %6, label %assert.exit.L19, label %assert.then.L19, !prof !5

assert.then.L19:                                  ; preds = %assert.exit.L18
  %7 = call ptr @__acrt_iob_func(i32 2)
  %8 = call i32 (ptr, ptr, ...) @fprintf(ptr %7, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L19:                                  ; preds = %assert.exit.L18
  store %struct.TestPacked zeroinitializer, ptr %tp, align 1
  %f1.addr3 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 0
  store i32 -2147483647, ptr %f1.addr3, align 4
  %f2.addr4 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 1
  store i64 9223372036854775807, ptr %f2.addr4, align 8
  %f1.addr5 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 0
  %9 = load i32, ptr %f1.addr5, align 4
  %10 = icmp eq i32 %9, -2147483647
  br i1 %10, label %assert.exit.L27, label %assert.then.L27, !prof !5

assert.then.L27:                                  ; preds = %assert.exit.L19
  %11 = call ptr @__acrt_iob_func(i32 2)
  %12 = call i32 (ptr, ptr, ...) @fprintf(ptr %11, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L19
  %f2.addr6 = getelementptr inbounds %struct.TestPacked, ptr %tp, i64 0, i32 1
  %13 = load i64, ptr %f2.addr6, align 8
  %14 = icmp eq i64 %13, 9223372036854775807
  br i1 %14, label %assert.exit.L28, label %assert.then.L28, !prof !5

assert.then.L28:                                  ; preds = %assert.exit.L27
  %15 = call ptr @__acrt_iob_func(i32 2)
  %16 = call i32 (ptr, ptr, ...) @fprintf(ptr %15, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L27
  %17 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree }
attributes #2 = { nounwind }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
