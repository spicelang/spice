; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { i32 }

@anon.string.0 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 't.copies == 1' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 't.copies == 2' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 't.copies == 3' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 't.copies == 4' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store i32 0, ptr %2, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %0, ptr noundef %1) #0 {
  %this = alloca ptr, align 8
  %old = alloca ptr, align 8
  %source = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %old, align 8
  %3 = load ptr, ptr %this, align 8
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %old, align 8
  store ptr %4, ptr %source, align 8
  %5 = load ptr, ptr %source, align 8
  %copies.addr = getelementptr inbounds %struct.Test, ptr %5, i64 0, i32 0
  %6 = load i32, ptr %copies.addr, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr %copies.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z8testRVO14Test(ptr dead_on_unwind noalias writable sret(%struct.Test) align 4 %0, %struct.Test noundef %1) #0 {
  %old = alloca %struct.Test, align 8
  store %struct.Test %1, ptr %old, align 4
  %3 = load %struct.Test, ptr %old, align 4
  store %struct.Test %3, ptr %0, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z8testRVO2RK4Test(ptr dead_on_unwind noalias writable sret(%struct.Test) align 4 %0, ptr noundef %1) #0 {
  %old = alloca ptr, align 8
  store ptr %1, ptr %old, align 8
  %3 = load ptr, ptr %old, align 8
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %0, ptr %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z8testRVO34Test(ptr dead_on_unwind noalias writable sret(%struct.Test) align 4 %0, %struct.Test noundef %1) #0 {
  %old = alloca %struct.Test, align 8
  %old1 = alloca %struct.Test, align 8
  store %struct.Test %1, ptr %old, align 4
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %old1, ptr %old)
  %3 = load %struct.Test, ptr %old1, align 4
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %old)
  store %struct.Test %3, ptr %0, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z8testRVO4RK4Test(ptr dead_on_unwind noalias writable sret(%struct.Test) align 4 %0, ptr noundef %1) #0 {
  %old = alloca ptr, align 8
  %old1 = alloca ptr, align 8
  store ptr %1, ptr %old, align 8
  %3 = load ptr, ptr %old, align 8
  store ptr %3, ptr %old1, align 8
  %4 = load ptr, ptr %old1, align 8
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %0, ptr %4)
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %t = alloca %struct.Test, align 8
  %arg.copy = alloca %struct.Test, align 8
  %t1 = alloca %struct.Test, align 8
  %t2 = alloca %struct.Test, align 8
  %arg.copy2 = alloca %struct.Test, align 8
  %t3 = alloca %struct.Test, align 8
  %t4 = alloca %struct.Test, align 8
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %t)
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %arg.copy, ptr %t)
  %1 = load %struct.Test, ptr %arg.copy, align 4
  call void @_Z8testRVO14Test(ptr dead_on_unwind writable sret(%struct.Test) align 4 %t1, %struct.Test noundef %1)
  %copies.addr = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %2 = load i32, ptr %copies.addr, align 4
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %assert.exit.L39, label %assert.then.L39, !prof !5

assert.then.L39:                                  ; preds = %0
  %4 = call ptr @__acrt_iob_func(i32 2)
  %5 = call i32 (ptr, ptr, ...) @fprintf(ptr %4, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %0
  call void @_Z8testRVO2RK4Test(ptr dead_on_unwind writable sret(%struct.Test) align 4 %t2, ptr noundef %t)
  %copies.addr1 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %6 = load i32, ptr %copies.addr1, align 4
  %7 = icmp eq i32 %6, 2
  br i1 %7, label %assert.exit.L42, label %assert.then.L42, !prof !5

assert.then.L42:                                  ; preds = %assert.exit.L39
  %8 = call ptr @__acrt_iob_func(i32 2)
  %9 = call i32 (ptr, ptr, ...) @fprintf(ptr %8, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L39
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 4 dereferenceable(4) %arg.copy2, ptr %t)
  %10 = load %struct.Test, ptr %arg.copy2, align 4
  call void @_Z8testRVO34Test(ptr dead_on_unwind writable sret(%struct.Test) align 4 %t3, %struct.Test noundef %10)
  %copies.addr3 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %11 = load i32, ptr %copies.addr3, align 4
  %12 = icmp eq i32 %11, 3
  br i1 %12, label %assert.exit.L45, label %assert.then.L45, !prof !5

assert.then.L45:                                  ; preds = %assert.exit.L42
  %13 = call ptr @__acrt_iob_func(i32 2)
  %14 = call i32 (ptr, ptr, ...) @fprintf(ptr %13, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L45:                                  ; preds = %assert.exit.L42
  call void @_Z8testRVO4RK4Test(ptr dead_on_unwind writable sret(%struct.Test) align 4 %t4, ptr noundef %t)
  %copies.addr4 = getelementptr inbounds %struct.Test, ptr %t, i64 0, i32 0
  %15 = load i32, ptr %copies.addr4, align 4
  %16 = icmp eq i32 %15, 4
  br i1 %16, label %assert.exit.L48, label %assert.then.L48, !prof !5

assert.then.L48:                                  ; preds = %assert.exit.L45
  %17 = call ptr @__acrt_iob_func(i32 2)
  %18 = call i32 (ptr, ptr, ...) @fprintf(ptr %17, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L48:                                  ; preds = %assert.exit.L45
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %t4)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %t3)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %t2)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %t1)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %t)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #3

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #4

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree }
attributes #3 = { nounwind }
attributes #4 = { cold noreturn nounwind }
attributes #5 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
