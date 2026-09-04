; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type {}

@printf.str.0 = private unnamed_addr constant [7 x i8] c"Ctor!\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Copy ctor!\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [7 x i8] c"Dtor!\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %0, ptr noundef %1) #0 {
  %this = alloca ptr, align 8
  %_ = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %_, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef %struct.Test @_Z6choosebRK4Test(i1 noundef zeroext %0, ptr noundef %1) #0 {
  %cond = alloca i1, align 1
  %ref = alloca ptr, align 8
  %3 = alloca %struct.Test, align 8
  %4 = alloca %struct.Test, align 8
  store i1 %0, ptr %cond, align 1
  store ptr %1, ptr %ref, align 8
  %5 = load i1, ptr %cond, align 1
  br i1 %5, label %cond.true.L11C12, label %cond.false.L11C12

cond.true.L11C12:                                 ; preds = %2
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %3)
  br label %cond.exit.L11C12

cond.false.L11C12:                                ; preds = %2
  %6 = load ptr, ptr %ref, align 8
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 1 %4, ptr %6)
  br label %cond.exit.L11C12

cond.exit.L11C12:                                 ; preds = %cond.false.L11C12, %cond.true.L11C12
  %cond.result = phi ptr [ %3, %cond.true.L11C12 ], [ %4, %cond.false.L11C12 ]
  %7 = load %struct.Test, ptr %cond.result, align 1
  ret %struct.Test %7
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %t = alloca %struct.Test, align 8
  %viaTemp = alloca %struct.Test, align 8
  %viaRef = alloca %struct.Test, align 8
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 1 %t)
  %1 = call noundef %struct.Test @_Z6choosebRK4Test(i1 noundef zeroext true, ptr noundef %t)
  store %struct.Test %1, ptr %viaTemp, align 1
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %viaTemp)
  %2 = call noundef %struct.Test @_Z6choosebRK4Test(i1 noundef zeroext false, ptr noundef %t)
  store %struct.Test %2, ptr %viaRef, align 1
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %viaRef)
  call void @_ZN4Test4dtorEv(ptr noundef nonnull align 1 %t)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
