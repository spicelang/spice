; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type {}

@printf.str.0 = private unnamed_addr constant [7 x i8] c"Ctor!\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Copy ctor!\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [7 x i8] c"Dtor!\0A\00", align 4

define private void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

define private void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %0, ptr noundef %1) {
  %this = alloca ptr, align 8
  %_ = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %_, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

define private void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret void
}

define private noundef %struct.Test @_Z3fooRK4Test(ptr noundef %0) {
  %result = alloca %struct.Test, align 8
  %t = alloca ptr, align 8
  %copy = alloca %struct.Test, align 8
  %2 = alloca %struct.Test, align 8
  store ptr %0, ptr %t, align 8
  %3 = load ptr, ptr %t, align 8
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %copy, ptr %3)
  br i1 false, label %cond.true.L12C12, label %cond.false.L12C12

cond.true.L12C12:                                 ; preds = %1
  br label %cond.exit.L12C12

cond.false.L12C12:                                ; preds = %1
  %4 = load ptr, ptr %t, align 8
  br label %cond.exit.L12C12

cond.exit.L12C12:                                 ; preds = %cond.false.L12C12, %cond.true.L12C12
  %cond.result = phi ptr [ %copy, %cond.true.L12C12 ], [ %4, %cond.false.L12C12 ]
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %2, ptr %cond.result)
  %5 = load %struct.Test, ptr %2, align 1
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %copy)
  ret %struct.Test %5
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  %1 = alloca %struct.Test, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %t)
  %2 = call noundef %struct.Test @_Z3fooRK4Test(ptr noundef %t)
  store %struct.Test %2, ptr %1, align 1
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %1)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %t)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
