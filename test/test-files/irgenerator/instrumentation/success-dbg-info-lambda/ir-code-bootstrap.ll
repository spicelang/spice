; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4, !dbg !0
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Doubled twice: %d\0A\00", align 4, !dbg !5
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Is even: %d\0A\00", align 4, !dbg !10

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %0, i32 noundef %1) #0 !dbg !20 {
  %fct = alloca { ptr, ptr, i64 }, align 8
  %value = alloca i32, align 4
    #dbg_declare(ptr %fct, !33, !DIExpression(), !35)
  store { ptr, ptr, i64 } %0, ptr %fct, align 8, !dbg !35
    #dbg_declare(ptr %value, !34, !DIExpression(), !36)
  store i32 %1, ptr %value, align 4, !dbg !36
  %3 = load i32, ptr %value, align 4, !dbg !37
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !37
  %captures = load ptr, ptr %4, align 8, !dbg !37
  %fct1 = load ptr, ptr %fct, align 8, !dbg !37
  %5 = call i32 %fct1(i32 %3, ptr %captures), !dbg !37
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !37
  %captures2 = load ptr, ptr %6, align 8, !dbg !37
  %fct3 = load ptr, ptr %fct, align 8, !dbg !37
  %7 = call i32 %fct3(i32 %5, ptr %captures2), !dbg !37
  ret i32 %7, !dbg !38
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 !dbg !39 {
  %counter = alloca i32, align 4
  %offset = alloca i64, align 8
  %captures = alloca { ptr, i64 }, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %increment = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr2 = alloca { ptr, ptr, i64 }, align 8
  %doubleFct = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr3 = alloca { ptr, ptr, i64 }, align 8
  %isEven = alloca { ptr, ptr, i64 }, align 8
    #dbg_declare(ptr %counter, !43, !DIExpression(), !57)
  store i32 10, ptr %counter, align 4, !dbg !57
    #dbg_declare(ptr %offset, !44, !DIExpression(), !58)
  store i64 5, ptr %offset, align 8, !dbg !58
  store ptr %counter, ptr %captures, align 8, !dbg !59
  %1 = load i64, ptr %offset, align 8, !dbg !59
  %2 = getelementptr inbounds nuw { ptr, i64 }, ptr %captures, i32 0, i32 1, !dbg !59
  store i64 %1, ptr %2, align 8, !dbg !59
  store ptr @_Z15lambda.L10C21.0v, ptr %fat.ptr, align 8, !dbg !59
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !59
  store ptr %captures, ptr %3, align 8, !dbg !59
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !59
  store i64 16, ptr %4, align 8, !dbg !59
    #dbg_declare(ptr %increment, !52, !DIExpression(), !59)
  %5 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !59
  store { ptr, ptr, i64 } %5, ptr %increment, align 8, !dbg !59
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %increment, i32 0, i32 1, !dbg !60
  %captures1 = load ptr, ptr %6, align 8, !dbg !60
  %fct = load ptr, ptr %increment, align 8, !dbg !60
  call void %fct(ptr %captures1), !dbg !60
  %7 = load i32, ptr %counter, align 4, !dbg !61
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %7), !dbg !61
  store ptr @_Z15lambda.L18C29.0i, ptr %fat.ptr2, align 8, !dbg !62
  %9 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 1, !dbg !62
  store ptr null, ptr %9, align 8, !dbg !62
  %10 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 2, !dbg !62
  store i64 0, ptr %10, align 8, !dbg !62
    #dbg_declare(ptr %doubleFct, !54, !DIExpression(), !62)
  %11 = load { ptr, ptr, i64 }, ptr %fat.ptr2, align 8, !dbg !62
  store { ptr, ptr, i64 } %11, ptr %doubleFct, align 8, !dbg !62
  %12 = load { ptr, ptr, i64 }, ptr %doubleFct, align 8, !dbg !63
  %13 = load i32, ptr %counter, align 4, !dbg !64
  %14 = call noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %12, i32 noundef %13), !dbg !64
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %14), !dbg !64
  store ptr @_Z15lambda.L24C27.0i, ptr %fat.ptr3, align 8, !dbg !65
  %16 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 1, !dbg !65
  store ptr null, ptr %16, align 8, !dbg !65
  %17 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 2, !dbg !65
  store i64 0, ptr %17, align 8, !dbg !65
    #dbg_declare(ptr %isEven, !56, !DIExpression(), !65)
  %18 = load { ptr, ptr, i64 }, ptr %fat.ptr3, align 8, !dbg !65
  store { ptr, ptr, i64 } %18, ptr %isEven, align 8, !dbg !65
  %19 = load i32, ptr %counter, align 4, !dbg !66
  %20 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %isEven, i32 0, i32 1, !dbg !66
  %captures4 = load ptr, ptr %20, align 8, !dbg !66
  %fct5 = load ptr, ptr %isEven, align 8, !dbg !66
  %21 = call i1 %fct5(i32 %19, ptr %captures4), !dbg !66
  %22 = zext i1 %21 to i32, !dbg !66
  %23 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %22), !dbg !66
  ret i32 0, !dbg !67
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L10C21.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !68 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !75
  %2 = load ptr, ptr %captures, align 8, !dbg !75
    #dbg_declare(ptr %2, !73, !DIExpression(), !75)
  %offset = getelementptr inbounds nuw { ptr, i64 }, ptr %2, i32 0, i32 1, !dbg !75
    #dbg_declare(ptr %offset, !74, !DIExpression(), !75)
  %3 = load ptr, ptr %2, align 8, !dbg !76
  %4 = load i32, ptr %3, align 4, !dbg !76
  %5 = add nsw i32 %4, 1, !dbg !76
  store i32 %5, ptr %3, align 4, !dbg !76
  %6 = load i64, ptr %offset, align 8, !dbg !77
  %7 = trunc i64 %6 to i32, !dbg !77
  %8 = load ptr, ptr %2, align 8, !dbg !77
  %9 = load i32, ptr %8, align 4, !dbg !77
  %10 = add nsw i32 %9, %7, !dbg !77
  store i32 %10, ptr %8, align 4, !dbg !77
  ret void, !dbg !78
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @_Z15lambda.L18C29.0i(i32 %0, ptr %1) #0 !dbg !79 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !84, !DIExpression(), !85)
  store i32 %0, ptr %x, align 4, !dbg !85
  store ptr %1, ptr %captures, align 8, !dbg !85
  %3 = load i32, ptr %x, align 4, !dbg !86
  %4 = mul nsw i32 %3, 2, !dbg !86
  ret i32 %4, !dbg !87
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i1 @_Z15lambda.L24C27.0i(i32 %0, ptr %1) #0 !dbg !88 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !94, !DIExpression(), !95)
  store i32 %0, ptr %x, align 4, !dbg !95
  store ptr %1, ptr %captures, align 8, !dbg !95
  %3 = load i32, ptr %x, align 4, !dbg !96
  %4 = srem i32 %3, 2, !dbg !96
  %5 = icmp eq i32 %4, 0, !dbg !97
  ret i1 %5, !dbg !97
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!13, !14, !15, !16, !17, !18}
!llvm.ident = !{!19}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !3, line: 15, type: !12, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false)
!3 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-lambda")
!4 = !{!0, !5, !10}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !3, line: 21, type: !7, isLocal: true, isDefinition: true)
!7 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 152, elements: !9)
!8 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!9 = !{}
!10 = !DIGlobalVariableExpression(var: !11, expr: !DIExpression())
!11 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !3, line: 25, type: !12, isLocal: true, isDefinition: true)
!12 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 104, elements: !9)
!13 = !{i32 8, !"PIC Level", i32 2}
!14 = !{i32 7, !"PIE Level", i32 2}
!15 = !{i32 7, !"uwtable", i32 2}
!16 = !{i32 7, !"frame-pointer", i32 0}
!17 = !{i32 7, !"Dwarf Version", i32 5}
!18 = !{i32 2, !"Debug Info Version", i32 3}
!19 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!20 = distinct !DISubprogram(name: "applyTwice", linkageName: "_Z10applyTwicePFiiEi", scope: !3, file: !3, line: 3, type: !21, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !32)
!21 = !DISubroutineType(types: !22)
!22 = !{!23, !24, !23}
!23 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!24 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !3, file: !3, size: 192, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !25, identifier: "_lambda")
!25 = !{!26, !29, !30}
!26 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !24, file: !3, baseType: !27, size: 64, align: 64)
!27 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64, align: 64, dwarfAddressSpace: 0)
!28 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !24, file: !3, baseType: !27, size: 64, align: 64, offset: 64)
!30 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !24, file: !3, baseType: !31, size: 64, align: 64, offset: 128)
!31 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!32 = !{!33, !34}
!33 = !DILocalVariable(name: "fct", arg: 1, scope: !20, file: !3, line: 3, type: !24)
!34 = !DILocalVariable(name: "value", arg: 2, scope: !20, file: !3, line: 3, type: !23)
!35 = !DILocation(line: 3, column: 19, scope: !20)
!36 = !DILocation(line: 3, column: 36, scope: !20)
!37 = !DILocation(line: 4, column: 20, scope: !20)
!38 = !DILocation(line: 5, column: 1, scope: !20)
!39 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !3, file: !3, line: 7, type: !40, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !42)
!40 = !DISubroutineType(types: !41)
!41 = !{!23}
!42 = !{!43, !44, !47, !52, !53, !54, !55, !56}
!43 = !DILocalVariable(name: "counter", scope: !39, file: !3, line: 8, type: !23)
!44 = !DILocalVariable(name: "offset", scope: !39, file: !3, line: 9, type: !45)
!45 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !46)
!46 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!47 = !DICompositeType(tag: DW_TAG_class_type, scope: !39, file: !3, line: 10, size: 128, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !48)
!48 = !{!49, !51}
!49 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !47, file: !3, line: 10, baseType: !50)
!50 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !23)
!51 = !DIDerivedType(tag: DW_TAG_member, name: "offset", scope: !47, file: !3, line: 10, baseType: !45, offset: 64)
!52 = !DILocalVariable(name: "increment", scope: !39, file: !3, line: 10, type: !24)
!53 = !DICompositeType(tag: DW_TAG_class_type, scope: !39, file: !3, line: 18, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !9)
!54 = !DILocalVariable(name: "doubleFct", scope: !39, file: !3, line: 18, type: !24)
!55 = !DICompositeType(tag: DW_TAG_class_type, scope: !39, file: !3, line: 24, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !9)
!56 = !DILocalVariable(name: "isEven", scope: !39, file: !3, line: 24, type: !24)
!57 = !DILocation(line: 8, column: 19, scope: !39)
!58 = !DILocation(line: 9, column: 25, scope: !39)
!59 = !DILocation(line: 10, column: 21, scope: !39)
!60 = !DILocation(line: 14, column: 5, scope: !39)
!61 = !DILocation(line: 15, column: 29, scope: !39)
!62 = !DILocation(line: 18, column: 29, scope: !39)
!63 = !DILocation(line: 21, column: 46, scope: !39)
!64 = !DILocation(line: 21, column: 57, scope: !39)
!65 = !DILocation(line: 24, column: 27, scope: !39)
!66 = !DILocation(line: 25, column: 36, scope: !39)
!67 = !DILocation(line: 26, column: 1, scope: !39)
!68 = distinct !DISubprogram(name: "lambda.L10C21", linkageName: "_Z15lambda.L10C21.0v", scope: !47, file: !3, line: 10, type: !69, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !72)
!69 = !DISubroutineType(types: !70)
!70 = !{!28, !71}
!71 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !47, size: 64, dwarfAddressSpace: 0)
!72 = !{!73, !74}
!73 = !DILocalVariable(name: "counter", scope: !68, file: !3, line: 8, type: !23)
!74 = !DILocalVariable(name: "offset", scope: !68, file: !3, line: 9, type: !45)
!75 = !DILocation(line: 10, column: 21, scope: !68)
!76 = !DILocation(line: 11, column: 9, scope: !68)
!77 = !DILocation(line: 12, column: 9, scope: !68)
!78 = !DILocation(line: 13, column: 5, scope: !68)
!79 = distinct !DISubprogram(name: "lambda.L18C29", linkageName: "_Z15lambda.L18C29.0i", scope: !53, file: !3, line: 18, type: !80, scopeLine: 18, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !83)
!80 = !DISubroutineType(types: !81)
!81 = !{!23, !82, !23}
!82 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !53, size: 64, dwarfAddressSpace: 0)
!83 = !{!84}
!84 = !DILocalVariable(name: "x", arg: 1, scope: !79, file: !3, line: 18, type: !23)
!85 = !DILocation(line: 18, column: 29, scope: !79)
!86 = !DILocation(line: 19, column: 20, scope: !79)
!87 = !DILocation(line: 20, column: 5, scope: !79)
!88 = distinct !DISubprogram(name: "lambda.L24C27", linkageName: "_Z15lambda.L24C27.0i", scope: !55, file: !3, line: 24, type: !89, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !93)
!89 = !DISubroutineType(types: !90)
!90 = !{!91, !92, !23}
!91 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!92 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !55, size: 64, dwarfAddressSpace: 0)
!93 = !{!94}
!94 = !DILocalVariable(name: "x", arg: 1, scope: !88, file: !3, line: 24, type: !23)
!95 = !DILocation(line: 24, column: 27, scope: !88)
!96 = !DILocation(line: 24, column: 42, scope: !88)
!97 = !DILocation(line: 24, column: 47, scope: !88)
