; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thread = type { { ptr, ptr }, i64 }

@printf.str.0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc range(i32 -2147483647, -2147483648) i32 @_Z3fibi(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 3
  br i1 %2, label %common.ret, label %if.exit.L4

common.ret.loopexit:                              ; preds = %if.exit.L4
  %3 = add nsw i32 %7, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %3, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.exit.L4:                                       ; preds = %1, %if.exit.L4
  %.tr4 = phi i32 [ %6, %if.exit.L4 ], [ %0, %1 ]
  %accumulator.tr3 = phi i32 [ %7, %if.exit.L4 ], [ 0, %1 ]
  %4 = add nsw i32 %.tr4, -1
  %5 = tail call fastcc i32 @_Z3fibi(i32 %4)
  %6 = add nsw i32 %.tr4, -2
  %7 = add nsw i32 %5, %accumulator.tr3
  %8 = icmp samesign ult i32 %.tr4, 5
  br i1 %8, label %common.ret.loopexit, label %if.exit.L4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
for.body.L11:
  %threads = alloca [8 x %struct.Thread], align 8
  %0 = alloca %struct.Thread, align 8
  %.fca.0.0.1.gep = getelementptr inbounds nuw i8, ptr %threads, i64 8
  %.fca.0.1.gep9 = getelementptr inbounds nuw i8, ptr %threads, i64 16
  %.fca.1.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 24
  %.fca.2.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 48
  %.fca.3.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 72
  %.fca.4.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 96
  %.fca.5.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 120
  %.fca.6.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 144
  %.fca.7.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 168
  %.fca.0.1.gep = getelementptr inbounds nuw i8, ptr %0, i64 8
  %.fca.1.gep = getelementptr inbounds nuw i8, ptr %0, i64 16
  %1 = getelementptr inbounds nuw i8, ptr %threads, i64 24
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(168) %1, i8 0, i64 168, i1 false)
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load = load ptr, ptr %0, align 8
  %.fca.0.1.load = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load, ptr %threads, align 8
  store ptr %.fca.0.1.load, ptr %.fca.0.0.1.gep, align 8
  store i64 %.fca.1.load, ptr %.fca.0.1.gep9, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %threads) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.1 = load ptr, ptr %0, align 8
  %.fca.0.1.load.1 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.1 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.1, ptr %.fca.1.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.1 = getelementptr inbounds nuw i8, ptr %threads, i64 32
  store ptr %.fca.0.1.load.1, ptr %.fca.1.insert5.fca.0.1.gep.1, align 8
  %.fca.1.insert5.fca.1.gep.1 = getelementptr inbounds nuw i8, ptr %threads, i64 40
  store i64 %.fca.1.load.1, ptr %.fca.1.insert5.fca.1.gep.1, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.1.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.2 = load ptr, ptr %0, align 8
  %.fca.0.1.load.2 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.2 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.2, ptr %.fca.2.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.2 = getelementptr inbounds nuw i8, ptr %threads, i64 56
  store ptr %.fca.0.1.load.2, ptr %.fca.1.insert5.fca.0.1.gep.2, align 8
  %.fca.1.insert5.fca.1.gep.2 = getelementptr inbounds nuw i8, ptr %threads, i64 64
  store i64 %.fca.1.load.2, ptr %.fca.1.insert5.fca.1.gep.2, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.2.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.3 = load ptr, ptr %0, align 8
  %.fca.0.1.load.3 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.3 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.3, ptr %.fca.3.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.3 = getelementptr inbounds nuw i8, ptr %threads, i64 80
  store ptr %.fca.0.1.load.3, ptr %.fca.1.insert5.fca.0.1.gep.3, align 8
  %.fca.1.insert5.fca.1.gep.3 = getelementptr inbounds nuw i8, ptr %threads, i64 88
  store i64 %.fca.1.load.3, ptr %.fca.1.insert5.fca.1.gep.3, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.3.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.4 = load ptr, ptr %0, align 8
  %.fca.0.1.load.4 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.4 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.4, ptr %.fca.4.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.4 = getelementptr inbounds nuw i8, ptr %threads, i64 104
  store ptr %.fca.0.1.load.4, ptr %.fca.1.insert5.fca.0.1.gep.4, align 8
  %.fca.1.insert5.fca.1.gep.4 = getelementptr inbounds nuw i8, ptr %threads, i64 112
  store i64 %.fca.1.load.4, ptr %.fca.1.insert5.fca.1.gep.4, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.4.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.5 = load ptr, ptr %0, align 8
  %.fca.0.1.load.5 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.5 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.5, ptr %.fca.5.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.5 = getelementptr inbounds nuw i8, ptr %threads, i64 128
  store ptr %.fca.0.1.load.5, ptr %.fca.1.insert5.fca.0.1.gep.5, align 8
  %.fca.1.insert5.fca.1.gep.5 = getelementptr inbounds nuw i8, ptr %threads, i64 136
  store i64 %.fca.1.load.5, ptr %.fca.1.insert5.fca.1.gep.5, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.5.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.6 = load ptr, ptr %0, align 8
  %.fca.0.1.load.6 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.6 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.6, ptr %.fca.6.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.6 = getelementptr inbounds nuw i8, ptr %threads, i64 152
  store ptr %.fca.0.1.load.6, ptr %.fca.1.insert5.fca.0.1.gep.6, align 8
  %.fca.1.insert5.fca.1.gep.6 = getelementptr inbounds nuw i8, ptr %threads, i64 160
  store i64 %.fca.1.load.6, ptr %.fca.1.insert5.fca.1.gep.6, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.6.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr poison }) #4
  %.fca.0.0.load.7 = load ptr, ptr %0, align 8
  %.fca.0.1.load.7 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.7 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.7, ptr %.fca.7.0.0.gep, align 8
  %.fca.1.insert5.fca.0.1.gep.7 = getelementptr inbounds nuw i8, ptr %threads, i64 176
  store ptr %.fca.0.1.load.7, ptr %.fca.1.insert5.fca.0.1.gep.7, align 8
  %.fca.1.insert5.fca.1.gep.7 = getelementptr inbounds nuw i8, ptr %threads, i64 184
  store i64 %.fca.1.load.7, ptr %.fca.1.insert5.fca.1.gep.7, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.7.0.0.gep) #4
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %threads) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.1.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.2.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.3.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.4.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.5.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.6.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %.fca.7.0.0.gep) #4
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

; Function Attrs: nofree nounwind
define private void @_Z15lambda.L12C29.0v() #2 {
  %1 = tail call fastcc i32 @_Z3fibi(i32 30)
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %1)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

declare void @_ZN6Thread4ctorEPFvE(ptr, { ptr, ptr }) local_unnamed_addr

declare void @_ZN6Thread3runEv(ptr) local_unnamed_addr

declare void @_ZN6Thread4joinEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #2

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #3

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
