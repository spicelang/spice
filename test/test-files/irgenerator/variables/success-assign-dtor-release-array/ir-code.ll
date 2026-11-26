; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.String = type { ptr, i64, i64 }
%struct.Holder = type { %struct.String }

@printf.str.0 = private unnamed_addr constant [10 x i8] c"dtor: %s\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [8 x i8] c"(empty)\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"first\00", align 4
@anon.string.2 = private unnamed_addr constant [7 x i8] c"second\00", align 4
@anon.string.3 = private unnamed_addr constant [6 x i8] c"third\00", align 4
@printf.str.1 = private unnamed_addr constant [14 x i8] c"items: %s %s\0A\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN6Holder4ctorERK6Holder(ptr noundef nonnull align 8 dereferenceable(24) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24) %3, ptr %1)
  ret void
}

declare void @_ZN6String4ctorERK6String(ptr noundef nonnull align 8 dereferenceable(24), ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN6Holder4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %0, ptr noundef %1) #1 {
  %this = alloca ptr, align 8
  %val = alloca ptr, align 8
  %3 = alloca %struct.String, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %val, align 8
  %4 = load ptr, ptr %this, align 8
  call void @_ZN6String4ctorEv(ptr noundef nonnull align 8 dereferenceable(24) %4)
  %5 = load ptr, ptr %this, align 8
  %s.addr = getelementptr inbounds %struct.Holder, ptr %5, i64 0, i32 0
  %6 = load ptr, ptr %val, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %3, ptr noundef %6)
  call void @_Z9op.assignR6StringRK6String(ptr %s.addr, ptr %3)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %3)
  ret void
}

declare void @_ZN6String4ctorEv(ptr noundef nonnull align 8 dereferenceable(24))

declare void @_ZN6String4ctorEPKc(ptr, ptr)

declare void @_Z9op.assignR6StringRK6String(ptr, ptr)

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %0) #1 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = call noundef zeroext i1 @_ZN6String7isEmptyEv(ptr noundef nonnull align 8 dereferenceable(24) %2)
  br i1 %3, label %cond.true.L16C26, label %cond.false.L16C26

cond.true.L16C26:                                 ; preds = %1
  br label %cond.exit.L16C26

cond.false.L16C26:                                ; preds = %1
  %4 = load ptr, ptr %this, align 8
  %5 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %4)
  br label %cond.exit.L16C26

cond.exit.L16C26:                                 ; preds = %cond.false.L16C26, %cond.true.L16C26
  %cond.result = phi ptr [ @anon.string.0, %cond.true.L16C26 ], [ %5, %cond.false.L16C26 ]
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %cond.result)
  %7 = load ptr, ptr %this, align 8
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %7)
  ret void
}

declare i1 @_ZN6String7isEmptyEv(ptr)

declare ptr @_ZN6String6getRawEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #3 {
  %items = alloca [2 x %struct.Holder], align 8
  %1 = alloca %struct.Holder, align 8
  %2 = alloca %struct.Holder, align 8
  %3 = alloca %struct.Holder, align 8
  %itemPtr = alloca ptr, align 8
  store [2 x %struct.Holder] zeroinitializer, ptr %items, align 8
  %4 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 0
  call void @_ZN6Holder4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %1, ptr noundef @anon.string.1)
  %5 = icmp eq ptr %4, %1
  br i1 %5, label %assign.move.end, label %assign.move

assign.move:                                      ; preds = %0
  call void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %4)
  call void @llvm.memcpy.p0.p0.i64(ptr %4, ptr %1, i64 24, i1 false)
  br label %assign.move.end

assign.move.end:                                  ; preds = %assign.move, %0
  %6 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 1
  call void @_ZN6Holder4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr noundef @anon.string.2)
  %7 = icmp eq ptr %6, %2
  br i1 %7, label %assign.move.end2, label %assign.move1

assign.move1:                                     ; preds = %assign.move.end
  call void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %6)
  call void @llvm.memcpy.p0.p0.i64(ptr %6, ptr %2, i64 24, i1 false)
  br label %assign.move.end2

assign.move.end2:                                 ; preds = %assign.move1, %assign.move.end
  %8 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 0
  call void @_ZN6Holder4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %3, ptr noundef @anon.string.3)
  %9 = icmp eq ptr %8, %3
  br i1 %9, label %assign.move.end4, label %assign.move3

assign.move3:                                     ; preds = %assign.move.end2
  call void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %8)
  call void @llvm.memcpy.p0.p0.i64(ptr %8, ptr %3, i64 24, i1 false)
  br label %assign.move.end4

assign.move.end4:                                 ; preds = %assign.move3, %assign.move.end2
  %10 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 1
  store ptr %10, ptr %itemPtr, align 8
  %11 = load ptr, ptr %itemPtr, align 8
  %12 = load ptr, ptr %itemPtr, align 8
  %13 = icmp eq ptr %11, %12
  br i1 %13, label %assign.move.end6, label %assign.move5

assign.move5:                                     ; preds = %assign.move.end4
  call void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %11)
  call void @llvm.memcpy.p0.p0.i64(ptr %11, ptr %12, i64 24, i1 false)
  br label %assign.move.end6

assign.move.end6:                                 ; preds = %assign.move5, %assign.move.end4
  %14 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 0
  %s.addr = getelementptr inbounds %struct.Holder, ptr %14, i64 0, i32 0
  %15 = load ptr, ptr %s.addr, align 8
  %16 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 1
  %s.addr7 = getelementptr inbounds %struct.Holder, ptr %16, i64 0, i32 0
  %17 = load ptr, ptr %s.addr7, align 8
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %15, ptr noundef %17)
  %19 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 0
  call void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %19)
  %20 = getelementptr inbounds [2 x %struct.Holder], ptr %items, i64 0, i32 1
  call void @_ZN6Holder4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %20)
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
