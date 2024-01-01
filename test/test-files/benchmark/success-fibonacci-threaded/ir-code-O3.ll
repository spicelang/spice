; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Thread = type { { ptr, ptr }, i64 }

@printf.str.0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_Z3fibi(i32 %0) unnamed_addr #0 {
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
  %8 = icmp ult i32 %.tr4, 5
  br i1 %8, label %common.ret.loopexit, label %if.exit.L4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
for.body.L11:
  %threads = alloca [8 x %struct.Thread], align 8
  %0 = alloca %struct.Thread, align 8
  %.fca.0.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 0, i32 0, i32 1
  %.fca.0.1.gep9 = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 0, i32 1
  %.fca.1.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 1, i32 0, i32 0
  %.fca.1.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 1, i32 0, i32 1
  %.fca.1.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 1, i32 1
  %.fca.2.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 2, i32 0, i32 0
  %.fca.2.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 2, i32 0, i32 1
  %.fca.2.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 2, i32 1
  %.fca.3.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 3, i32 0, i32 0
  %.fca.3.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 3, i32 0, i32 1
  %.fca.3.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 3, i32 1
  %.fca.4.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 4, i32 0, i32 0
  %.fca.4.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 4, i32 0, i32 1
  %.fca.4.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 4, i32 1
  %.fca.5.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 5, i32 0, i32 0
  %.fca.5.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 5, i32 0, i32 1
  %.fca.5.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 5, i32 1
  %.fca.6.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 6, i32 0, i32 0
  %.fca.6.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 6, i32 0, i32 1
  %.fca.6.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 6, i32 1
  %.fca.7.0.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 7, i32 0, i32 0
  %.fca.7.0.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 7, i32 0, i32 1
  %.fca.7.1.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 7, i32 1
  %.fca.0.1.gep = getelementptr inbounds %struct.Thread, ptr %0, i64 0, i32 0, i32 1
  %.fca.1.gep = getelementptr inbounds %struct.Thread, ptr %0, i64 0, i32 1
  %1 = getelementptr inbounds i8, ptr %threads, i64 24
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(168) %1, i8 0, i64 168, i1 false)
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load = load ptr, ptr %0, align 8
  %.fca.0.1.load = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load, ptr %threads, align 8
  store ptr %.fca.0.1.load, ptr %.fca.0.0.1.gep, align 8
  store i64 %.fca.1.load, ptr %.fca.0.1.gep9, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %threads) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.1 = load ptr, ptr %0, align 8
  %.fca.0.1.load.1 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.1 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.1, ptr %.fca.1.0.0.gep, align 8
  store ptr %.fca.0.1.load.1, ptr %.fca.1.0.1.gep, align 8
  store i64 %.fca.1.load.1, ptr %.fca.1.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.1.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.2 = load ptr, ptr %0, align 8
  %.fca.0.1.load.2 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.2 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.2, ptr %.fca.2.0.0.gep, align 8
  store ptr %.fca.0.1.load.2, ptr %.fca.2.0.1.gep, align 8
  store i64 %.fca.1.load.2, ptr %.fca.2.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.2.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.3 = load ptr, ptr %0, align 8
  %.fca.0.1.load.3 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.3 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.3, ptr %.fca.3.0.0.gep, align 8
  store ptr %.fca.0.1.load.3, ptr %.fca.3.0.1.gep, align 8
  store i64 %.fca.1.load.3, ptr %.fca.3.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.3.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.4 = load ptr, ptr %0, align 8
  %.fca.0.1.load.4 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.4 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.4, ptr %.fca.4.0.0.gep, align 8
  store ptr %.fca.0.1.load.4, ptr %.fca.4.0.1.gep, align 8
  store i64 %.fca.1.load.4, ptr %.fca.4.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.4.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.5 = load ptr, ptr %0, align 8
  %.fca.0.1.load.5 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.5 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.5, ptr %.fca.5.0.0.gep, align 8
  store ptr %.fca.0.1.load.5, ptr %.fca.5.0.1.gep, align 8
  store i64 %.fca.1.load.5, ptr %.fca.5.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.5.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.6 = load ptr, ptr %0, align 8
  %.fca.0.1.load.6 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.6 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.6, ptr %.fca.6.0.0.gep, align 8
  store ptr %.fca.0.1.load.6, ptr %.fca.6.0.1.gep, align 8
  store i64 %.fca.1.load.6, ptr %.fca.6.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.6.0.0.gep) #4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %0, { ptr, ptr } { ptr @_Z15lambda.L12C29.0v, ptr undef }) #4
  %.fca.0.0.load.7 = load ptr, ptr %0, align 8
  %.fca.0.1.load.7 = load ptr, ptr %.fca.0.1.gep, align 8
  %.fca.1.load.7 = load i64, ptr %.fca.1.gep, align 8
  store ptr %.fca.0.0.load.7, ptr %.fca.7.0.0.gep, align 8
  store ptr %.fca.0.1.load.7, ptr %.fca.7.0.1.gep, align 8
  store i64 %.fca.1.load.7, ptr %.fca.7.1.gep, align 8
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.7.0.0.gep) #4
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %threads) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.1.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.2.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.3.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.4.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.5.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.6.0.0.gep) #4
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(8) %.fca.7.0.0.gep) #4
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
