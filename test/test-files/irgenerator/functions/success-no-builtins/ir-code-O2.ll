; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 4

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) uwtable
define void @_Z9fillBytesPhhm(ptr nofree noundef writeonly align 1 captures(none) %0, i8 noundef zeroext %1, i64 noundef %2) local_unnamed_addr #0 {
  %.not = icmp eq i64 %2, 0
  br i1 %.not, label %for.exit.L6, label %for.body.L6.preheader

for.body.L6.preheader:                            ; preds = %3
  tail call void @llvm.memset.p0.i64(ptr align 1 %0, i8 %1, i64 %2, i1 false)
  br label %for.exit.L6

for.exit.L6:                                      ; preds = %for.body.L6.preheader, %3
  ret void
}

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write) uwtable
define void @_Z24fillBytesWithoutBuiltinsPhhm(ptr nofree noundef writeonly align 1 captures(none) %0, i8 noundef zeroext %1, i64 noundef %2) local_unnamed_addr #1 {
  %.not = icmp eq i64 %2, 0
  br i1 %.not, label %for.exit.L15, label %iter.check

iter.check:                                       ; preds = %3
  %min.iters.check = icmp ult i64 %2, 4
  br i1 %min.iters.check, label %for.body.L15.preheader, label %vector.main.loop.iter.check

vector.main.loop.iter.check:                      ; preds = %iter.check
  %min.iters.check4 = icmp ult i64 %2, 32
  br i1 %min.iters.check4, label %vec.epilog.ph, label %vector.ph

vector.ph:                                        ; preds = %vector.main.loop.iter.check
  %n.mod.vf = and i64 %2, 28
  %n.vec = and i64 %2, -32
  %broadcast.splatinsert = insertelement <16 x i8> poison, i8 %1, i64 0
  %broadcast.splat = shufflevector <16 x i8> %broadcast.splatinsert, <16 x i8> poison, <16 x i32> zeroinitializer
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %4 = getelementptr inbounds i8, ptr %0, i64 %index
  %5 = getelementptr inbounds nuw i8, ptr %4, i64 16
  store <16 x i8> %broadcast.splat, ptr %4, align 1
  store <16 x i8> %broadcast.splat, ptr %5, align 1
  %index.next = add nuw i64 %index, 32
  %6 = icmp eq i64 %index.next, %n.vec
  br i1 %6, label %middle.block, label %vector.body, !llvm.loop !5

middle.block:                                     ; preds = %vector.body
  %cmp.n = icmp eq i64 %2, %n.vec
  br i1 %cmp.n, label %for.exit.L15, label %vec.epilog.iter.check

vec.epilog.iter.check:                            ; preds = %middle.block
  %min.epilog.iters.check = icmp eq i64 %n.mod.vf, 0
  br i1 %min.epilog.iters.check, label %for.body.L15.preheader, label %vec.epilog.ph, !prof !8

vec.epilog.ph:                                    ; preds = %vector.main.loop.iter.check, %vec.epilog.iter.check
  %vec.epilog.resume.val = phi i64 [ %n.vec, %vec.epilog.iter.check ], [ 0, %vector.main.loop.iter.check ]
  %n.vec6 = and i64 %2, -4
  %broadcast.splatinsert7 = insertelement <4 x i8> poison, i8 %1, i64 0
  %broadcast.splat8 = shufflevector <4 x i8> %broadcast.splatinsert7, <4 x i8> poison, <4 x i32> zeroinitializer
  br label %vec.epilog.vector.body

vec.epilog.vector.body:                           ; preds = %vec.epilog.vector.body, %vec.epilog.ph
  %index9 = phi i64 [ %vec.epilog.resume.val, %vec.epilog.ph ], [ %index.next10, %vec.epilog.vector.body ]
  %7 = getelementptr inbounds i8, ptr %0, i64 %index9
  store <4 x i8> %broadcast.splat8, ptr %7, align 1
  %index.next10 = add nuw i64 %index9, 4
  %8 = icmp eq i64 %index.next10, %n.vec6
  br i1 %8, label %vec.epilog.middle.block, label %vec.epilog.vector.body, !llvm.loop !9

vec.epilog.middle.block:                          ; preds = %vec.epilog.vector.body
  %cmp.n11 = icmp eq i64 %2, %n.vec6
  br i1 %cmp.n11, label %for.exit.L15, label %for.body.L15.preheader

for.body.L15.preheader:                           ; preds = %iter.check, %vec.epilog.iter.check, %vec.epilog.middle.block
  %i.03.ph = phi i64 [ 0, %iter.check ], [ %n.vec, %vec.epilog.iter.check ], [ %n.vec6, %vec.epilog.middle.block ]
  br label %for.body.L15

for.body.L15:                                     ; preds = %for.body.L15.preheader, %for.body.L15
  %i.03 = phi i64 [ %10, %for.body.L15 ], [ %i.03.ph, %for.body.L15.preheader ]
  %9 = getelementptr inbounds i8, ptr %0, i64 %i.03
  store i8 %1, ptr %9, align 1
  %10 = add nuw i64 %i.03, 1
  %exitcond.not = icmp eq i64 %10, %2
  br i1 %exitcond.not, label %for.exit.L15, label %for.body.L15, !llvm.loop !10

for.exit.L15:                                     ; preds = %for.body.L15, %middle.block, %vec.epilog.middle.block, %3
  ret void
}

; Function Attrs: mustprogress nofree noinline norecurse nounwind uwtable
define noundef i32 @main() local_unnamed_addr #2 {
  %destWithoutBuiltins = alloca [4 x i8], align 4
  %.fca.3.gep = getelementptr inbounds nuw i8, ptr %destWithoutBuiltins, i64 3
  store i32 0, ptr %destWithoutBuiltins, align 4
  call void @_Z24fillBytesWithoutBuiltinsPhhm(ptr noundef nonnull align 1 %destWithoutBuiltins, i8 noundef zeroext 7, i64 noundef 4)
  %1 = load i8, ptr %.fca.3.gep, align 1
  %2 = zext i8 %1 to i32
  %3 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef 7, i32 noundef %2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #4

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) uwtable }
attributes #1 = { nofree norecurse nosync nounwind memory(argmem: write) uwtable "no-builtins" }
attributes #2 = { mustprogress nofree noinline norecurse nounwind uwtable }
attributes #3 = { nofree nounwind }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = distinct !{!5, !6, !7}
!6 = !{!"llvm.loop.isvectorized", i32 1}
!7 = !{!"llvm.loop.unroll.runtime.disable"}
!8 = !{!"branch_weights", i32 4, i32 28}
!9 = distinct !{!9, !6, !7}
!10 = distinct !{!10, !7, !6}
