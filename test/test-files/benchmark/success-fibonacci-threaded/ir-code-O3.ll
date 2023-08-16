; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Thread = type { ptr, i64 }

@printf.str.0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_Z3fibi(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 3
  br i1 %2, label %common.ret, label %if.exit.L4

common.ret.loopexit:                              ; preds = %if.exit.L4
  %3 = add i32 %7, 1
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
  %7 = add i32 %5, %accumulator.tr3
  %8 = icmp ult i32 %.tr4, 5
  br i1 %8, label %common.ret.loopexit, label %if.exit.L4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
for.body.L11:
  %threads = alloca [8 x %struct.Thread], align 8
  %0 = alloca %struct.Thread, align 8
  %.fca.1.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 1, i32 0
  %.fca.2.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 2, i32 0
  %.fca.3.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 3, i32 0
  %.fca.4.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 4, i32 0
  %.fca.5.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 5, i32 0
  %.fca.6.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 6, i32 0
  %.fca.7.0.gep = getelementptr inbounds [8 x %struct.Thread], ptr %threads, i64 0, i64 7, i32 0
  %1 = getelementptr inbounds i8, ptr %threads, i64 16
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(112) %1, i8 0, i64 112, i1 false)
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %threads, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %threads) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.1.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.1.0.gep) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.2.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.2.0.gep) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.3.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.3.0.gep) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.4.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.4.0.gep) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.5.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.5.0.gep) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.6.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.6.0.gep) #5
  call void @_ZN6Thread4ctorEPFvE(ptr nonnull %0, ptr nonnull @_Z15lambda.L12C29.0v) #5
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %.fca.7.0.gep, ptr noundef nonnull align 8 dereferenceable(16) %0, i64 16, i1 false)
  call void @_ZN6Thread3runEv(ptr nonnull %.fca.7.0.gep) #5
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  call void @_ZN6Thread4joinEv(ptr nonnull %threads) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.1.0.gep) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.2.0.gep) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.3.0.gep) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.4.0.gep) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.5.0.gep) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.6.0.gep) #5
  call void @_ZN6Thread4joinEv(ptr nonnull %.fca.7.0.gep) #5
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

declare void @_ZN6Thread4ctorEPFvE(ptr, ptr) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #3

declare void @_ZN6Thread3runEv(ptr) local_unnamed_addr

declare void @_ZN6Thread4joinEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #2

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #4

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nounwind }
