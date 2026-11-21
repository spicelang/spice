; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thread = type { %struct.Lambda, i64 }
%struct.Lambda = type { { ptr, ptr, i64 }, ptr, i64 }
%struct.Result = type { i1, %struct.Error }
%struct.Error = type { i32, ptr }

@printf.str.1 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'started.isOk()' evaluated to false.\0A\00", align 4
@stderr = external local_unnamed_addr global ptr, align 8
@anon.string.1 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'joined.isOk()' evaluated to false.\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 4
@str = private unnamed_addr constant [25 x i8] c"Starting all threads ...\00", align 1

; Function Attrs: nofree nosync nounwind memory(none) uwtable
define internal fastcc noundef i32 @_Z3fibi(i32 noundef %0) unnamed_addr #0 {
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
  %5 = tail call fastcc noundef i32 @_Z3fibi(i32 noundef %4)
  %6 = add nsw i32 %.tr4, -2
  %7 = add i32 %5, %accumulator.tr3
  %8 = icmp samesign ult i32 %.tr4, 5
  br i1 %8, label %common.ret.loopexit, label %if.exit.L4
}

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define noundef i32 @main() local_unnamed_addr #1 {
for.body.L13:
  %threads = alloca [8 x %struct.Thread], align 8
  %0 = alloca %struct.Thread, align 8
  %started = alloca %struct.Result, align 8
  %joined = alloca %struct.Result, align 8
  %.fca.1.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 48
  %.fca.2.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 96
  %.fca.3.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 144
  %.fca.4.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 192
  %.fca.5.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 240
  %.fca.6.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 288
  %.fca.7.0.0.0.gep = getelementptr inbounds nuw i8, ptr %threads, i64 336
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  %1 = getelementptr inbounds nuw i8, ptr %threads, i64 48
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(336) %1, i8 0, i64 336, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %threads, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %threads) #7
  %2 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %2, label %for.head.L13, label %assert.then.L20, !prof !5

for.head.L13:                                     ; preds = %for.body.L13
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.1.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.1.0.0.0.gep) #7
  %3 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %3, label %for.head.L13.1, label %assert.then.L20, !prof !5

for.head.L13.1:                                   ; preds = %for.head.L13
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.2.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.2.0.0.0.gep) #7
  %4 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %4, label %for.head.L13.2, label %assert.then.L20, !prof !5

for.head.L13.2:                                   ; preds = %for.head.L13.1
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.3.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.3.0.0.0.gep) #7
  %5 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %5, label %for.head.L13.3, label %assert.then.L20, !prof !5

for.head.L13.3:                                   ; preds = %for.head.L13.2
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.4.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.4.0.0.0.gep) #7
  %6 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %6, label %for.head.L13.4, label %assert.then.L20, !prof !5

for.head.L13.4:                                   ; preds = %for.head.L13.3
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.5.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.5.0.0.0.gep) #7
  %7 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %7, label %for.head.L13.5, label %assert.then.L20, !prof !5

for.head.L13.5:                                   ; preds = %for.head.L13.4
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.6.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.6.0.0.0.gep) #7
  %8 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %8, label %for.head.L13.6, label %assert.then.L20, !prof !5

for.head.L13.6:                                   ; preds = %for.head.L13.5
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %0, { ptr, ptr, i64 } noundef { ptr @_Z15lambda.L14C29.0v, ptr null, i64 0 }) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(48) %.fca.7.0.0.0.gep, ptr noundef nonnull align 8 dereferenceable(48) %0, i64 48, i1 false)
  call void @_ZN6Thread3runEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %started, ptr noundef nonnull align 8 dereferenceable(48) %.fca.7.0.0.0.gep) #7
  %9 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %started) #7
  br i1 %9, label %for.head.L13.7, label %assert.then.L20, !prof !5

for.head.L13.7:                                   ; preds = %for.head.L13.6
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %threads) #7
  %10 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %10, label %for.head.L22, label %assert.then.L25, !prof !5

assert.then.L20:                                  ; preds = %for.head.L13.6, %for.head.L13.5, %for.head.L13.4, %for.head.L13.3, %for.head.L13.2, %for.head.L13.1, %for.head.L13, %for.body.L13
  %11 = load ptr, ptr @stderr, align 8
  %12 = call i64 @fwrite(ptr nonnull @anon.string.0, i64 65, i64 1, ptr %11) #8
  call void @exit(i32 1)
  unreachable

for.head.L22:                                     ; preds = %for.head.L13.7
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.1.0.0.0.gep) #7
  %13 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %13, label %for.head.L22.1, label %assert.then.L25, !prof !5

for.head.L22.1:                                   ; preds = %for.head.L22
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.2.0.0.0.gep) #7
  %14 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %14, label %for.head.L22.2, label %assert.then.L25, !prof !5

for.head.L22.2:                                   ; preds = %for.head.L22.1
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.3.0.0.0.gep) #7
  %15 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %15, label %for.head.L22.3, label %assert.then.L25, !prof !5

for.head.L22.3:                                   ; preds = %for.head.L22.2
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.4.0.0.0.gep) #7
  %16 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %16, label %for.head.L22.4, label %assert.then.L25, !prof !5

for.head.L22.4:                                   ; preds = %for.head.L22.3
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.5.0.0.0.gep) #7
  %17 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %17, label %for.head.L22.5, label %assert.then.L25, !prof !5

for.head.L22.5:                                   ; preds = %for.head.L22.4
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.6.0.0.0.gep) #7
  %18 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %18, label %for.head.L22.6, label %assert.then.L25, !prof !5

for.head.L22.6:                                   ; preds = %for.head.L22.5
  call void @_ZN6Thread4joinEv(ptr dead_on_unwind nonnull writable sret(%struct.Result) align 8 %joined, ptr noundef nonnull align 8 dereferenceable(48) %.fca.7.0.0.0.gep) #7
  %19 = call noundef zeroext i1 @_ZN6ResultIbE4isOkEv(ptr noundef nonnull align 8 dereferenceable(24) %joined) #7
  br i1 %19, label %for.head.L22.7, label %assert.then.L25, !prof !5

for.head.L22.7:                                   ; preds = %for.head.L22.6
  %20 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0

assert.then.L25:                                  ; preds = %for.head.L22.6, %for.head.L22.5, %for.head.L22.4, %for.head.L22.3, %for.head.L22.2, %for.head.L22.1, %for.head.L22, %for.head.L13.7
  %21 = load ptr, ptr @stderr, align 8
  %22 = call i64 @fwrite(ptr nonnull @anon.string.1, i64 64, i64 1, ptr %21) #8
  call void @exit(i32 1)
  unreachable
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nofree nounwind uwtable
define internal void @_Z15lambda.L14C29.0v(ptr nofree readnone captures(none) %0) #3 {
  %2 = tail call fastcc noundef i32 @_Z3fibi(i32 noundef 30)
  %3 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 noundef %2)
  ret void
}

declare void @_ZN6Thread4ctorEPFvE(ptr, { ptr, ptr, i64 }) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

declare void @_ZN6Thread3runEv(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8, ptr) local_unnamed_addr

declare i1 @_ZN6ResultIbE4isOkEv(ptr) local_unnamed_addr

; Function Attrs: cold nofree noreturn nounwind
declare void @exit(i32) local_unnamed_addr #5

declare void @_ZN6Thread4joinEv(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #6

attributes #0 = { nofree nosync nounwind memory(none) uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nofree nounwind uwtable }
attributes #4 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { cold nofree noreturn nounwind }
attributes #6 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
attributes #7 = { nounwind }
attributes #8 = { cold }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
